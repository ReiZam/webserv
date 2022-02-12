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
	std::string allPath = block_config.getRoot() + request.GetUri().AllPath() + block_config.getIndex();
	
	(void)config;
	if (!exist_file(allPath))
		this->_response_code = 404;
	return (allPath);
}


void	Response::write_error_body(Request &request, ServerConfig const &config, BlockConfig const &block_config)
{
	(void)request;
	(void)config;
	(void)block_config;
}

void	Response::write_body(Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path)
{
	this->_body = get_file_content(path);
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
	std::string path = this->findPath(request, config, block_config);
	
	std::cout << "current_path: " << path << std::endl;
	this->_header.SetValue("Server", config.getServerName());
	this->_header.SetValue("Date", GetDate());
	if (this->_response_code == 200)
	{
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
	
	if (this->_response_code != 200)
	{
		this->_body = gen_html_error_page(this->_response_code);
		this->_header.SetValue("Content-Type", "text/html; charset=UTF-8");
	}

	this->_header.SetValue("Content-Length", SSTR(this->_body.size()));
	this->_start_line = "HTTP/1.1 " + gen_status_code(this->_response_code);
	this->_http_response = this->_start_line + "\r\n" + this->_header.HtoStr() + "\r\n" + this->_body;
}