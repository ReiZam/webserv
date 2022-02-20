#include "../webserv.hpp"

Request::Request() : _step(START), _scode(200), _start_line(""), _header(Header()), _body(),  _ishost(false) {}

Request::Request(const Request& cop) : _scode(200), _start_line(cop._start_line), _header(cop._header), _body(cop._body) {}

Request&	Request::operator=(const Request& cop)
{
	_step = cop._step;
	_scode = cop._scode;
	_start_line = cop._start_line;
	_header = cop._header;
	_body = cop._body;
	_method = cop._method;
	_version = cop._version;
	_raw_path = cop._raw_path;
	_uri = cop._uri;
	_ishost = cop._ishost;
	return *this;
}

Request::~Request() {}

void	Request::ParseHeader(std::string http_header)
{
	std::size_t pos;
	
	//	Get the Request line
	if (_step == START)
	{
		pos = http_header.find("\n");
		_start_line = http_header.substr(0, pos - 1);
		http_header.erase(0, pos+1);

		_method = _start_line.substr(0, _start_line.find(" "));
		_version = _start_line.substr(_start_line.find(" H") + 1);
		_raw_path = _start_line.substr(_method.size() + 1, _start_line.substr(_method.size() + 1).find(" "));
		_uri = Uri(_raw_path);
		_step = HEADER;
	}
	//	Let Parse the Header
	while (_step == HEADER && (pos = http_header.find(":")) != std::string::npos)
	{
		std::string	line = http_header.substr(0, http_header.find("\n"));
		if (line.length() && *(line.end() - 1) == '\n')
			line.resize(line.size()-1);
		if (line.length() && *(line.end() - 1) == '\r')
			line.resize(line.size()-1);
		std::string key = (line.empty()) ? "" : line.substr(0, pos);
		std::string value = (line.size() <= pos+2) ? "" :  line.substr(pos+1);
		while (value[0] == ' ')
			value.erase(0,1);
		if (key.empty() || value.empty())
			_scode = BAD_REQUEST;
		http_header.erase(0, http_header.find("\n")+1);
		_header.SetValue(key, value);
	}
	if (_uri.GetHost().empty() && _header.GetValue("Host").empty())
		_scode = BAD_REQUEST;
	_step = (this->GetMethod() == "POST" || this->GetMethod() == "DELETE") ? BODY : END;
}

bool	Request::ValidPost(ServerConfig const &config, std::string string_request)
{
	std::string content_type = this->_header.GetValue("Content-Type");
	BlockConfig const &block_config = config.getBlockConfigFromURI(this->_uri);

	if (content_type.empty())
	{
		_scode = BAD_REQUEST;
		return (false);
	}
	else if (static_cast<unsigned long>(block_config.getBodySize()) <= string_request.size())
	{
		_scode = REQUEST_ENTITY_TOO_LARGE;
		return (false);
	}
	if (content_type.find("application/x-www-form-urlencoded") != std::string::npos)
		return (true);
	else if (content_type.find("multipart/form-data; boundary=") != std::string::npos)
	{
		if (content_type.find("boundary=") == std::string::npos)
		{
			_scode = BAD_REQUEST;
			return (false);
		}
		std::string boundary = content_type.substr(content_type.find("boundary="));
		boundary = boundary.substr(9, boundary.find("; ")) + "--\r\n";
		
		return (ends_with(string_request, boundary));
	}
	return (false);
}

bool	Request::BadChunked(const std::string next, const size_t count) const
{
	std::string::const_iterator it = next.begin();
	for (; it!=next.end() && std::isxdigit(*it); ++it)
		continue ;
	if ((it != next.begin() && (*it == '\r' || *it == '\n'))
		|| count == 0)
		return false;
	return true;
}

bool	Request::ParseChunked(std::string request_body)
{
	if (!ends_with(request_body, "0\r\n\r\n") || request_body.size() < 5)
		return (false);
	while (request_body.find("0\r\n\r\n") != std::string::npos && request_body.size() != 5)
	{
		std::string	line = request_body.substr(0, request_body.find("\n"));
		if (line.length() && *(line.end() - 1) == '\n')
			line.resize(line.size()-1);
		if (line.length() && line.find("\r"))
			line.resize(line.size()-1);
		std::size_t count = std::strtoul(line.c_str(), NULL, 16);
		if (line.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos
			|| BadChunked(request_body.substr(line.size() + 2 + count + 2), count)
			|| request_body.size() < std::strtoul(line.c_str(), NULL, 16))
		{
			_scode = BAD_REQUEST;
			return (false);
		}
		request_body.erase(0, line.size() + 2);
		for (size_t i = 0;i < count;i++)
			this->_body.push_back(static_cast<unsigned char>(request_body[i]));
		line.clear();
		request_body.erase(0, count);
		if (request_body[0] == '\r')
			request_body.erase(0,1);
		if (request_body[0] == '\n')
			request_body.erase(0,1);
	}
	_step = END;
	return (true);
}

void	Request::ValidBody(ServerConfig const &config)
{
	BlockConfig const &block_config = config.getBlockConfigFromURI(this->_uri);

	if (_header.GetHeader()["Content-Length"].c_str())
	{
		std::size_t	length = strtol(_header.GetValue("Content-Length").c_str(), NULL, 10);
		if (length != this->_body.size())
			_scode = BAD_REQUEST;
		else if (static_cast<unsigned long>(block_config.getBodySize()) <= this->_body.size())
			_scode = REQUEST_ENTITY_TOO_LARGE;
	}
	this->_step = END;
}
