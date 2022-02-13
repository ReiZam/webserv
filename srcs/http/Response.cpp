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
	allPath = block_config.getRoot() + (ends_with(allPath, "/") ? allPath.substr(0, allPath.size() - 1) : allPath);
	if (exist_file(allPath))
		return (allPath);
	this->_response_code = NOT_FOUND;
	allPath += "/" + block_config.getIndex();
	this->_response_code = exist_file(allPath) ? OK : NOT_FOUND;
	return (allPath);
}


void	Response::write_error_body(BlockConfig const &block_config)
{
	std::string	content_type;
	std::string	last_modified_date;
	
	if (!block_config.getErrorPages()[this->_response_code].empty() && exist_file(block_config.getErrorPages()[this->_response_code]))
	{
		std::string path = block_config.getErrorPages()[this->_response_code];
		
		this->_body = read_file(path.c_str());
		content_type = get_file_type(path);
		last_modified_date = GetLastModifiedDate(path);
	}
	else
	{
		this->_body = gen_html_error_page(this->_response_code);
		content_type = "text/html";
		last_modified_date = GetDate();
	}

	this->_header.SetValue("Content-Type", content_type);
	this->_header.SetValue("Last-Modified", last_modified_date);
}

void	Response::write_body(Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path)
{
	this->_body = read_file(path.c_str());
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
	
	this->_header.SetValue("Server", WEBSERV_VERSION);
	this->_header.SetValue("Date", GetDate());
	if (this->isValidResponseCode())
	{
		if (block_config.getMethodsAllowed()[request_method] == true)
		{
			std::string path = this->findPath(request, config, block_config);

			this->write_body(request, config, block_config, path);
		}
		else
			this->_response_code = METHOD_NOT_ALLOWED;
	}
	if (!this->isValidResponseCode())
		this->write_error_body(block_config);
	this->_header.SetValue("Content-Length", SSTR(this->_body.size()));
	this->_raw_header = "HTTP/1.1 " + gen_status_code(this->_response_code) + "\r\n" + this->_header.HtoStr() + "\r\n";
}