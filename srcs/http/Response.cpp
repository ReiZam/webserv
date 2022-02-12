#include "../webserv.hpp"

Response::Response() : _header(), _start_line(), _body(), _http_response(), _response_code(200) {}

Response::~Response() {}

Response::Response(Response const &cop)
{
    *this = cop;
}

Response&   Response::operator=(Response const &cop)
{
	this->_header = cop._header;
	this->_start_line = cop._start_line;
	this->_body = cop._body;
	this->_http_response = cop._http_response;
    this->_response_code = cop._response_code;
    return *this;
}

std::string	Response::findPath(Request &request, ServerConfig const &config, BlockConfig const &block_config)
{
	std::string allPath = request.GetUri().AllPath();

	try
	{
		LocationConfig const &location_config = config.getLocationConfigFromURI(request.GetUri());
		
		allPath = allPath.substr(location_config.getLocationName().size());
		if (allPath.empty() || allPath[0] != '/')
			allPath = "/" + allPath;
	} catch(const std::exception& e) {}
	allPath = block_config.getRoot() + allPath;
	if (exist_file(allPath))
		return (allPath);
	else
		this->_response_code = NOT_FOUND;
	allPath = (ends_with(allPath, "/") ? allPath : (allPath + "/")) + block_config.getIndex();
	this->_response_code = exist_file(allPath) ? OK : NOT_FOUND;
	return (allPath);
}


void	Response::write_error_body(BlockConfig const &block_config)
{
	std::string path = !block_config.getErrorPages()[this->_response_code].empty() && exist_file(block_config.getErrorPages()[this->_response_code]) ? block_config.getErrorPages()[this->_response_code] : "";
	
	this->_body =  path.empty() ? gen_html_error_page(this->_response_code) : get_file_content(path);
	this->_header.SetValue("Content-Type", path.empty() ? "text/html" : get_file_type(path));
	this->_header.SetValue("Last-Modified", path.empty() ? GetDate() : GetLastModifiedDate(path));
}

void	Response::write_body(Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path)
{
	this->_body = get_file_content(path);
	this->_header.SetValue("Content-Type", get_file_type(path));
	this->_header.SetValue("Last-Modified", GetLastModifiedDate(path));
	(void)request;
	(void)config;
	(void)block_config;
	(void)path;
}

void	Response::generateResponse(Request &request, ServerConfig const &config)
{
	BlockConfig const &block_config = config.getBlockConfigFromURI(request.GetUri());
	std::string request_method = request.GetMethod();
	
	this->_header.SetValue("Server", config.getServerName());
	this->_header.SetValue("Date", GetDate());
	if (this->_response_code == OK)
	{
		if (block_config.getMethodsAllowed()[request_method] == true)
		{
			std::string path = this->findPath(request, config, block_config);

			this->write_body(request, config, block_config, path);
			if (request_method.compare("GET") == 0)
			{

			}
			else if (request_method.compare("POST") == 0)
			{

			}
			else if (request_method.compare("DELETE") == 0)
			{
				
			}
		}
		else
			this->_response_code = METHOD_NOT_ALLOWED;
	}
	if (this->_response_code != OK)
		this->write_error_body(block_config);
	this->_header.SetValue("Content-Length", SSTR(this->_body.size()));
	this->_start_line = "HTTP/1.1 " + gen_status_code(this->_response_code);
	this->_http_response = this->_start_line + "\r\n" + this->_header.HtoStr() + "\r\n" + this->_body;
}