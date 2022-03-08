#include "../webserv.hpp"

Request::Request() : _step(START), _scode(200), _start_line(""), _boundary(), _header(), _body(), _ishost(false) {}

Request::Request(const Request& cop)
{
	*this = cop;
}

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
	_boundary = cop._boundary;
	_current_config = cop._current_config;
	return *this;
}

Request::~Request() {}

bool	Request::CheckStartLine()
{
	if (!_version.empty() && _version != "HTTP/1.1" && _version != "HTTP/1.0")
		_scode = HTTP_VERSION_NOT_SUPPORTED;
	else if (_raw_path.empty() || _method.empty() || !check_forbidden_characters_string(_raw_path, " ") || !check_forbidden_characters_string(_method, " "))
		_scode = BAD_REQUEST;
	else if (_method != "DELETE" && _method != "POST" && _method != "GET")
		_scode = NOT_IMPLEMENTED;
	else if (_raw_path.size() >= 2048)
		_scode = REQUEST_URI_TOO_LONG;
	return (_scode == OK);
}

bool	Request::CheckGlobalHeader(std::string http_header)
{
	if (http_header.find("\r\n") != 0 || (_uri.GetHost().empty() && _header.GetValue("Host").empty()))
		_scode = BAD_REQUEST;
	else if (!_header.GetValue("Content-Length").empty())
	{
		long int length = strtol(_header.GetValue("Content-Length").c_str(), NULL, 10);
		
		if (length < 0)
			_scode = BAD_REQUEST;
	}
	return (_scode == OK);
}

void	Request::ParseStartLine()
{
	_method = _start_line.substr(0, _start_line.find(" "));
	_version = _start_line.substr(_start_line.rfind(" ") + 1);
	_raw_path = _start_line.substr(_method.size() + 1, _start_line.rfind(" ") - _method.size() - 1);
}

void	Request::ParseHeader(std::string http_header)
{
	std::size_t pos = 0;
	//	Get the Request line
	if (_step == START)
	{
		pos = http_header.find("\r\n");
		_start_line = http_header.substr(0, pos);
		ParseStartLine();
		if (CheckStartLine())
		{
			_uri = Uri(_raw_path);
			_step = HEADER;
		}
		else return ;
		http_header.erase(0, pos + 2);
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
		if (key.empty() || value.empty() || !_header.GetValue(key).empty())
			_scode = BAD_REQUEST;
		http_header.erase(0, http_header.find("\n")+1);
		_header.SetValue(key, value);
	}
	
	if (CheckGlobalHeader(http_header))	
		_step = this->GetMethod() == "POST" ? BODY : END;
}

void	Request::chooseServerConfig(std::vector<ServerConfig> const &configs)
{
	std::string host = this->_header.GetValue("Host");

	if (!host.empty() && configs.size() > 1)
	{
		std::vector<std::string> host_parts = parse_server_name_parts(host);

		for (std::vector<ServerConfig>::const_iterator it = configs.begin();it != configs.end();it++)
		{
			if ((*it).getServerName().compare(host) == 0)
			{
				this->_current_config = *it;
				return ;
			}
		}
		for (std::vector<ServerConfig>::const_iterator it = configs.begin();it != configs.end();it++)
		{
			std::vector<std::string> parts = parse_server_name_parts((*it).getServerName());
			size_t i;

			if (parts.size() == 0 || parts[0] != "*") continue ;
			for (i = 1;i < parts.size() && i < host_parts.size();i++)
				if (parts[i] != host_parts[i])
					break ;
			if (i == parts.size() && i == host_parts.size())
			{
				this->_current_config = *it;
				return ;
			}
		}
		for (std::vector<ServerConfig>::const_iterator it = configs.begin();it != configs.end();it++)
		{
			std::vector<std::string> parts = parse_server_name_parts((*it).getServerName());
			size_t i;

			if (parts.size() == 0 || parts[parts.size() - 1] != "*") continue ;
			for (i = 0;i < parts.size() - 1 && i < host_parts.size();i++)
				if (parts[i] != host_parts[i])
					break ;
			if (i + 1 == parts.size() && i + 1 == host_parts.size())
			{
				this->_current_config = *it;
				return ;
			}
		}
	}
	this->_current_config = configs[0];
}

bool	Request::ValidPost(std::string string_request)
{
	std::string content_type = this->_header.GetValue("Content-Type");
	BlockConfig const &block_config = _current_config.getBlockConfigFromURI(this->_uri);

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
		std::string tmp = content_type.substr(content_type.find("boundary="));
		this->_boundary = "--" + tmp.substr(9, tmp.find("; "));
		tmp = this->_boundary + "--\r\n";
		return (ends_with(string_request, tmp));
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

void	Request::ValidBody()
{
	std::string content = this->_header.GetValue("Content-Length");
	BlockConfig const &block_config = _current_config.getBlockConfigFromURI(this->_uri);

	if (content.size())
	{
		std::size_t	length = strtol(_header.GetValue("Content-Length").c_str(), NULL, 10);
		
		if (length != this->_body.size())
			_scode = BAD_REQUEST;
		else if (static_cast<unsigned long>(block_config.getBodySize()) <= this->_body.size())
			_scode = REQUEST_ENTITY_TOO_LARGE;
	}
	else
		_scode = LENGTH_REQUIRED;
	this->_step = END;
}
