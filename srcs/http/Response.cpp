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
		allPath += "/";
		if (exist_directory(allPath))
		{
			requestPath += "/";
			this->_response_code = MOVED_PERMANENTLY;
			this->_header.SetValue("Location", requestPath);
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
		this->_header.SetValue("Content-Type", config.getMediaType(path));
		this->_header.SetValue("Last-Modified", GetLastModifiedDate(path));
	}
	else
	{
		this->_body = gen_html_error_page(this->_response_code);
		this->_header.SetValue("Content-Type", "text/html");
	}
}

void	Response::write_body_with_file(ServerConfig const &config, std::string path)
{
	if (this->isValidResponseCode())
	{
		this->_body = read_file(path.c_str());
		this->_header.SetValue("Content-Type", config.getMediaType(path));
		this->_header.SetValue("Last-Modified", GetLastModifiedDate(path));
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

void	Response::generateResponse(Request &request, ServerConfig const &config)
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
			
			this->write_body_with_file(config, path);
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
