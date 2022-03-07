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
	if (ends_with(block_config.getRoot(), "/") && allPath[0] == '/')
		allPath = allPath.substr(1);
	allPath = block_config.getRoot() + allPath;
	
	if (!exist_file(allPath) && !ends_with(requestPath, "/"))
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


bool	Response::write_error_page_from_errcode(ServerConfig const &config, BlockConfig const &block_config, int error_code)
{
	std::string path = block_config.getErrorPages()[error_code];

	if (!path.empty())
	{
		if (exist_file(path))
		{
			this->_body = read_file(path.c_str());
			this->_header.SetValue("Content-Type", config.getMediaType(path, "application/octet-stream"));
			this->_header.SetValue("Last-Modified", GetLastModifiedDate(path));
			return (true);
		}
		else
			this->_response_code = 500;
	}
	return (false);
}

void	Response::write_error_body(ServerConfig const &config, BlockConfig const &block_config)
{
	if (this->write_error_page_from_errcode(config, block_config, this->_response_code) == false)
	{
		this->_body = gen_html_error_page(this->_response_code);
		this->_header.SetValue("Content-Type", "text/html");
	}
}

char**	Response::generate_cgi_env(Client &client, Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path)
{
	std::string request_method = request.GetMethod();
	std::map<std::string, std::string> cgi_env_map;
	std::string tmp = path;
	Uri uri(tmp);
	std::string script_name = uri.GetPath()[uri.GetPath().size() - 1];

	if (!script_name.empty() && script_name[0] == '/')
		script_name = script_name.substr(1);
	(void)block_config;
	cgi_env_map["CONTENT_LENGTH"] = SSTR(request.GetBody().size());
	cgi_env_map["CONTENT_TYPE"] = request_method == "POST" ? "application/x-www-form-urlencoded" : "text/plain";
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
		std::string tmp = (*it).first + "=" + (*it).second;

		cgi_env[i++] = strdup(tmp.c_str());
	}
	cgi_env[i] = 0;
	return (cgi_env);
}

void	Response::parse_cgi_response(std::string cgi_response)
{
	if (cgi_response.find("\r\n\r\n") == std::string::npos)
		throw WebservException("CGI", "CGI Execution failed");
	Header header = parse_header(cgi_response.substr(0, cgi_response.find("\r\n\r\n") + 4));
	std::map<std::string,std::string> header_map = header.GetHeader();
	std::string body = cgi_response.substr(cgi_response.find("\r\n\r\n") + 4);

	for (std::map<std::string, std::string>::iterator it = header_map.begin();it != header_map.end();it++)
		this->_header.SetValue((*it).first, (*it).second);
	if (!body.empty())
		this->_body = string_to_uchar_vec(body);
}

void	Response::execute_cgi(Client &client, Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path, std::string cgi_path)
{
	char	**cgi_env = generate_cgi_env(client, request, config, block_config, path);
	char	*args[] = {(char*)cgi_path.c_str(), (char*)path.c_str(), NULL};
	size_t	i = 0;
	pid_t	pid;
	int		ret;
	FILE	*file_in = tmpfile();
	FILE	*file_out = tmpfile();
	int		tmp_in;
	int		tmp_out;
	int		file_in_fd = fileno(file_in);
	int		file_out_fd = fileno(file_out);

	tmp_in = dup(0);
	tmp_out = dup(1);
	ret = 0;
	if (client.getRequestBody().size() > 0)
		write(file_in_fd, client.getRequestBody().c_str(), client.getRequestBody().size());
	lseek(file_in_fd, 0, SEEK_SET);
	if ((pid = fork()) < 0)
		throw WebservException("CGI", "fork() failed");
	if (pid == 0)
	{
		try
		{
			if (dup2(file_in_fd, 0) < 0)
				throw WebservException("CGI", "dup2() failed");
			if (dup2(file_out_fd, 1) < 0)
				throw WebservException("CGI", "dup2() failed");
			if ((ret = execve(args[0], &args[0], cgi_env) < 0))
				throw WebservException("CGI", "CGI Execution failed");
			exit(ret);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			exit(ret);
		}
	}
	else
	{
		while (cgi_env[i])
			free(cgi_env[i++]);
		free(cgi_env);
		waitpid(pid, (int*)0, 0);
		lseek(file_out_fd, 0, SEEK_SET);
		std::string cgi_response = read_fd(file_out_fd);
		parse_cgi_response(cgi_response);
	}
	dup2(tmp_in, 0);
	dup2(tmp_out, 1);
	fclose(file_in);
	fclose(file_out);
	close(file_in_fd);
	close(file_out_fd);
	close(tmp_in);
	close(tmp_out);
}

void	Response::write_body_with_file(Client &client, Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path)
{
	if (this->isValidResponseCode())
	{
		(void)request;
		(void)client;
		if (block_config.isCgiExtension(get_path_ext(path)))
		{
			try
			{
				this->execute_cgi(client, request, config, block_config, path, block_config.GetCgiExtension(get_path_ext(path)));
			}
			catch(const std::exception& e)
			{
				this->_response_code = BAD_GATEWAY;
			}
		}
		else
		{
			this->_body = read_file(path.c_str());
			this->_header.SetValue("Content-Type", config.getMediaType(path, "application/octet-stream"));
			this->_header.SetValue("Last-Modified", GetLastModifiedDate(path));
		}
	}
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

void	Response::save_uploaded_files(std::string client_body, Request &request, BlockConfig const &block_config)
{
	std::string upload_directory = block_config.getFileUploadDirectory();
	std::string content_type = request.GetHeader().GetValue("Content-Type");
	std::string boundary = request.GetBoundary();
	std::string end_boundary = boundary + "--\r\n";

	if (content_type.find("multipart/form-data") != std::string::npos && exist_directory(upload_directory))
	{
		try
		{
			while (client_body.find(boundary) != std::string::npos && client_body.find(end_boundary) != 0)
			{
				client_body = client_body.substr(client_body.find(boundary) + boundary.size() + 2);
				std::string tmp = client_body.substr(0, client_body.find(boundary) - 2);

				if (tmp.find("\r\n\r\n") == std::string::npos)
					throw std::exception();
					
				Header tmp_header = parse_header(tmp.substr(0, tmp.find("\r\n\r\n") + 4));
				std::string tmp_body = tmp.substr(tmp.find("\r\n\r\n") + 4, tmp.size());
				std::string content_disposition = tmp_header.GetValue("Content-Disposition");
				
				if (content_disposition.empty() || content_disposition.find("form-data") == std::string::npos)
					throw std::exception();
				if (content_disposition.find("filename=\"") != std::string::npos)
				{
					std::string content_type = tmp_header.GetValue("Content-Type");
					std::string filename;
					std::string dst;
					
					if (content_type.empty())
						throw std::exception();
					filename = content_disposition.substr(content_disposition.find("filename=\"") + 10);

					if (filename.find("\"") == std::string::npos)
						throw std::exception();
					filename = filename.substr(0, filename.find("\""));
					
					dst = upload_directory + (ends_with(upload_directory, "/") ? "" : "/") + filename;

					std::ofstream outfile(dst.c_str());

					outfile << tmp_body;

					outfile.close();
				}
				client_body = client_body.substr(client_body.find(boundary));
			}
		}
		catch (const std::exception& e)
		{
			this->_response_code = BAD_REQUEST;
		}
	}
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
	if (this->_response_code == 200 && request_method == "POST")
		this->save_uploaded_files(client.getRequestBody(), request, block_config);
	if (!path.empty() && this->_response_code == NOT_FOUND && exist_directory(path))
	{
		if (block_config.isAutoIndex())
			this->write_body_autoindex(path);
		else if (this->write_error_page_from_errcode(config, block_config, 600))
			this->_response_code = 200;
	}
	if (request.GetHeader().IsValueSetTo("Connection", "keep-alive"))
		this->_header.SetValue("Connection", "keep-alive");

	if (this->_response_code >= 300 && this->_response_code <= 505)
		this->write_error_body(config, block_config);
	if (this->_response_code == OK && request_method == "DELETE" && exist_file(path))
	{
		if (remove(path.c_str()) != 0)
			this->_response_code = NO_CONTENT;
		else
			this->_response_code = OK;
	}
	if (request_method == "HEAD")
		this->_body.clear();
	else
		this->_header.SetValue("Content-Length", SSTR(this->_body.size()));
	this->_raw_header = "HTTP/1.1 " + gen_status_code(this->_response_code) + "\r\n" + this->_header.HtoStr() + "\r\n";
}
