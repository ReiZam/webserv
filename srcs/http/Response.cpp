#include "../webserv.hpp"

Response::Response() : _header(), _raw_header(), _body(), _response_code(200) {}

Response::~Response() {}

Response::Response(Response const &cop)
{
    *this = cop;
}

Response&   Response::operator=(Response const &cop)
{
	this->_header = cop._header;
	this->_body = cop._body;
	this->_raw_header = cop._raw_header;
    this->_response_code = cop._response_code;
    return *this;
}

std::string	Response::parsePath(Request &request, ServerConfig const &config, BlockConfig const &block_config)
{
	std::string requestPath = request.GetUri().AllPath();
	std::string allPath = requestPath;

	try
	{
		LocationConfig const &location_config = config.getLocationConfigFromURI(request.GetUri());

		if (location_config.isValueSet("root"))
			allPath = allPath.substr(location_config.getLocationName().size());
	}
	catch(const std::exception& e)
	{}
	allPath = block_config.getRoot() + allPath;
	if (!exist_file(allPath) && !ends_with(allPath, "/"))
	{
		if (exist_directory(allPath + "/"))
		{
			allPath += "/";
			this->_response_code = MOVED_PERMANENTLY;
			this->_header.SetValue("Location", requestPath + "/");
			return (allPath);
		}
		else
			this->_response_code = NOT_FOUND;
		return (allPath);
	}
	else if (exist_file(allPath))
	{
		this->_response_code = OK;
		return (allPath);
	}
	else
	{
		std::string allPathIndex = allPath + block_config.getIndex();

		if (!exist_file(allPathIndex))
		{
			this->_response_code = NOT_FOUND;
			return (allPath);
		}
		else
			this->_response_code = OK;
		return (allPathIndex);
	}
}


void	Response::write_error_body(ServerConfig const &config, BlockConfig const &block_config)
{
	if (!block_config.getErrorPages()[this->_response_code].empty() && exist_file(block_config.getErrorPages()[this->_response_code]))
	{
		std::string path = block_config.getErrorPages()[this->_response_code];
		
		this->_body = read_file(path.c_str());
		this->_header.SetValue("Content-Type", config.getMediaType(path, "application/octet-stream"));
		this->_header.SetValue("Last-Modified", GetLastModifiedDate(path));
	}
	else
	{
		this->_body = gen_html_error_page(this->_response_code);
		this->_header.SetValue("Content-Type", "text/html");
	}
}

char**	Response::generate_cgi_env(Client &client, Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path)
{
	std::map<std::string, std::string> cgi_env_map;
	std::string tmp = path;
	Uri uri(tmp);
	std::string script_name = uri.GetPath()[uri.GetPath().size() - 1];

	if (!script_name.empty() && script_name[0] == '/')
		script_name = script_name.substr(1);
		
	(void)block_config;
	cgi_env_map["CONTENT_LENGTH"] = SSTR(request.GetBody().size());
	cgi_env_map["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	cgi_env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	cgi_env_map["PATH_INFO"] = request.GetUri().AllPath();
	cgi_env_map["QUERY_STRING"] = request.GetUri().AllQuery();
	cgi_env_map["REMOTE_ADDR"] = client.getIp();
	cgi_env_map["REQUEST_METHOD"] = request.GetMethod();
	cgi_env_map["SCRIPT_NAME"] = script_name;
	cgi_env_map["SERVER_NAME"] = config.getServerName();
	cgi_env_map["SERVER_PORT"] = SSTR(config.getPort());
	cgi_env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	cgi_env_map["SERVER_SOFTWARE"] = WEBSERV_VERSION;
	cgi_env_map["REQUEST_URI"] = request.GetUri().AllUri();
	cgi_env_map["REDIRECT_STATUS"] = SSTR(200);
	cgi_env_map["SCRIPT_FILENAME"] = path;

	char	**cgi_env = (char**)malloc(sizeof(char*) * (cgi_env_map.size() + 1));
	size_t	i = 0;

	for (std::map<std::string, std::string>::iterator it = cgi_env_map.begin();it != cgi_env_map.end();++it)
	{
		std::string tmp = ((*it).first + "=" + (*it).second);

		cgi_env[i++] = strdup(tmp.c_str());
	}
	cgi_env[i] = 0;
	return (cgi_env);
}	

void	Response::execute_cgi(Client &client, Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path)
{
	char	**args = (char**)malloc(sizeof(char*) * (2));
	pid_t	pid;
	int		status;
	int		ret;
	int		_pipe[2];
	int		_pipe2[2];
	char	**cgi_env = generate_cgi_env(client, request, config, block_config, path);

	// args[0] = (char*)"/home/debian/webserv/test.perl";
	args[0] = (char*)"/usr/bin/php-cgi7.3";
	args[1] = strdup(path.c_str());
	args[2] = 0;
	ret = 0;
	if (pipe(_pipe) < 0)
		throw WebservException("CGI", "pipe() failed");
	if (pipe(_pipe2) < 0)
		throw WebservException("CGI", "pipe() failed");
	if ((pid = fork()) < 0)
		throw WebservException("CGI", "fork() failed");
	if (pid == 0)
	{
		try
		{	
			if (dup2(_pipe2[0], 0) < 0)
				throw WebservException("CGI", "dup2() failed");
			if (dup2(_pipe[1], 1) < 0)
				throw WebservException("CGI", "dup2() failed");
			if ((ret = execve(args[0], args + 1, cgi_env) < 0))
				throw WebservException("CGI", "CGI Execution failed");
			exit(ret);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			exit(ret);
		}
	}
	else
	{
		close(_pipe2[0]);
		write(_pipe2[1], client.getRequestBody().c_str(), client.getRequestBody().size());
		close(_pipe2[1]);
		close(_pipe[1]);

		std::string current_result = read_fd(_pipe[0]);
		std::cout << current_result << std::endl;

		std::string header = current_result.substr(0, current_result.find("\r\n\r\n"));
		std::string body = current_result.substr(current_result.find("\r\n\r\n") + 4, current_result.size());
		this->_body = string_to_uchar_vec(body);
		std::string key = header.substr(0, header.find(":"));
		std::string value = header.substr(header.find(": ") + 2, header.size());
		this->_header.SetValue(key, value);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
	}
}

void	Response::write_body_with_file(Client &client, Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path)
{
	if (this->isValidResponseCode())
	{
		(void)request;
		(void)client;
		if (block_config.isCgiExtension(get_path_ext(path)))
			this->execute_cgi(client, request, config, block_config, path);
		else
		{
			this->_body = read_file(path.c_str());
			this->_header.SetValue("Content-Type", config.getMediaType(path, "application/octet-stream"));
			this->_header.SetValue("Last-Modified", GetLastModifiedDate(path));
		}
	}
	(void)block_config;
}

void	Response::write_body_autoindex(std::string path)
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(path.c_str())) != NULL)
	{
		std::map<std::string, std::string> content;
		std::string folder_content;	

		while ((ent = readdir (dir)) != NULL)
		{
			std::string item_name = ent->d_name;

			if (item_name.compare(".") != 0)
				content[item_name] = item_name;
		}

		for (std::map<std::string, std::string>::iterator it = content.begin();it != content.end();++it)
		{
			std::string item_name = (*it).first;
			std::string tmp = "<a href=\"" + item_name + "\">" + item_name + "</a><br>";

			folder_content.append(tmp);
		}
		this->_header.SetValue("Content-Type", "text/html");
		this->_body = string_to_uchar_vec("<!DOCTYPE html><html><head><title>Webserv - Index of " + path + "</title><head><body><h1>Webserv - Index of " + path + "</h1><pre><hr>" + folder_content + "<hr></pre></body></html>");
		this->_response_code = OK;
		closedir(dir);
	}
	else
		this->_response_code = 500;
}

void	Response::generateResponse(Client &client, Request &request, ServerConfig const &config)
{
	BlockConfig const &block_config = config.getBlockConfigFromURI(request.GetUri());
	std::string request_method = request.GetMethod();
	std::string	path;
	
	this->_header.SetValue("Server", WEBSERV_VERSION);
	this->_header.SetValue("Date", GetDate());
	if (this->isValidResponseCode())
	{
		if (block_config.getMethodsAllowed()[request_method] == true)
		{
			path = this->parsePath(request, config, block_config);
			
			this->write_body_with_file(client, request, config, block_config, path);
		}
		else
			this->_response_code = METHOD_NOT_ALLOWED;
	}

	if (!path.empty() && this->_response_code == NOT_FOUND && exist_directory(path))
	{
		if (block_config.isAutoIndex())
			this->write_body_autoindex(path);
		else
			this->_response_code = FORBIDDEN;
	}
	if (request.GetHeader().IsValueSetTo("Connection", "keep-alive"))
		this->_header.SetValue("Connection", "keep-alive");
	if (this->_response_code >= 400 && this->_response_code <= 505)
		this->write_error_body(config, block_config);
	this->_header.SetValue("Content-Length", SSTR(this->_body.size()));
	this->_raw_header = "HTTP/1.1 " + gen_status_code(this->_response_code) + "\r\n" + this->_header.HtoStr() + "\r\n";
}
