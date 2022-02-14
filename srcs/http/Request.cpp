#include "../webserv.hpp"

/*
	REQUEST HTTP
*/
Request::Request() : _step(START), _scode(200), _start_line(""), _header(Header()), _body(""), _uri(Uri()), _ishost(false)
{}

// Request::Request(const Server& conf) : _serv(conf), _step(START), _scode(200), _start_line(""), _header(Header()), _body(""), _uri(Uri()), _ishost(false) {}

Request::Request(const Request& cop) : _step(cop._step), _scode(cop._scode), _start_line(cop._start_line), _header(cop._header),
										_body(cop._body), _uri(cop._uri), _ishost(cop._ishost)
{}

Request&	Request::operator=(const Request& cop)
{
	_step = cop._step;
	_scode = cop._scode;
	_start_line = cop._start_line;
	_header = cop._header;
	_body = cop._body;
	_uri = cop._uri;
	_ishost = cop._ishost;
	return *this;
}

Request::~Request()
{}

void		Request::ValidStartLine(std::string href)
{
	std::string start(href);
	std::string method = start.substr(0, start.find(" "));
	if (method.length() == 0 || (method != "GET" && method != "HEAD" && method != "POST"
		&& method != "PUT" && method != "DELETE" && method != "CONNECT"
		&& method != "OPTIONS" && method != "TRACE" && method != "PATCH"))
		_scode = BAD_REQUEST;
	else if (method != "GET" && method != "POST" && method != "DELETE")
		_scode = METHOD_NOT_ALLOWED;
	start.erase(0, start.find(" ")+1);
	if (start.substr(0, start.find(" ")).length() == 0)
		_scode = BAD_REQUEST;
	//	uri is parse in her class
	start.erase(0, start.find(" ")+1);
	if (start.length() == 0 || (start != "HTTP/1.0" && start != "HTTP/1.1"))
		_scode = HTTP_VERSION_NOT_SUPPORTED;
	start.clear();
}

// bool	ValidContentLenght(std::string lenght)
// {
// 	if (lenght.size() && lenght.find_first_not_of("0123456789"))
// 	{
// 		if (_serv.)
// 	}
// 	return false;
// }

/*
	@param:	 string-> the position of the following potential body part
			 size_t-> current chunk pos
	@brief:	 "return false;" if the next if valid or if count equal 0
			 otherwise "return true;"
*/
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
/*
	@param:	 copy of the body received
	@brief:	 work good if the body is complete
	@except: may throw if the chunked's hexadecimal part has not good init
				like -> "3R\r\n" or "2A\r"
	@ref:	 https://datatracker.ietf.org/doc/html/rfc7230#section-4
*/
void	Request::ParseBodyChunked(std::string cbody)
{
	while (cbody.find("0\r\n\r\n") != std::string::npos && cbody.size() != 5)
	{
		std::string	line = cbody.substr(0, cbody.find("\n"));
		if (line.length() && *(line.end() - 1) == '\n')
			line.resize(line.size()-1);
		if (line.length() && line.find("\r"))
			line.resize(line.size()-1);
		std::size_t count = std::strtoul(line.c_str(), NULL, 16);
		if (line.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos
			|| BadChunked(cbody.substr(line.size() + 2 + count + 2), count)
			|| cbody.size() < std::strtoul(line.c_str(), NULL, 16))
		{
			throw(Request::RequestExcept("Body Chunked bad formating."));
		}
		cbody.erase(0, line.size() + 2);
		_body += cbody.substr(0, count);
		line.clear();
		cbody.erase(0, count);
		if (cbody[0] == '\r')
			cbody.erase(0,1);
		if (cbody[0] == '\n')
			cbody.erase(0,1);
	}
}

void	Request::ParseBody(std::string body, const ServerConfig &config)
{
	BlockConfig const &block_config = config.getBlockConfigFromURI(this->GetUri());

	if (_header.GetValue("Transfer-Encoding").find("chunked") != std::string::npos)
	{
		ParseBodyChunked(body);
	}
	else if (_header.GetHeader()["Content-Length"].c_str())
	{
		std::size_t	lenght = strtoul(_header.GetValue("Content-Length").c_str(), NULL, 10);
		if (lenght == body.size())
		{
			_scode = OK;
			_body = body;
		}
		else if (static_cast<unsigned long>(block_config.getBodySize()) > body.size())
			_scode = REQUEST_ENTITY_TOO_LARGE;
	}
	else if (!body.empty())
		_scode = LENGTH_REQUIRED;
	_step = END;
}

/*
	@brief: Parse the Header Request step by step or in once time
	@param: href = brut header by Client
*/
void		Request::ParseRequest(std::string href, const ServerConfig &config)
{
	std::size_t 	pos;
	// static std::string	buf += href; // maybe...

	//	Get the Request line
	if (_step == START) {
		pos = href.find("\n");
		_start_line = href.substr(0, pos - 1);
		std::string raw_path = _start_line.substr(GetMethod().size()+1, _start_line.substr(GetMethod().size()+1).find(" "));
		//	raw_path = Uri.AllPath()
		_uri = Uri(raw_path);
		href.erase(0, pos+1);
		_step = HEADER;
	}
	//	Let Parse the Header
	while (_step == HEADER && (pos = href.find(":")) != std::string::npos)
	{
		std::string	line = href.substr(0, href.find("\n"));
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
		href.erase(0, href.find("\n")+1);
		_header.SetValue(key, value);
	}
	if (_step == HEADER && href[0] == '\r' && href[1] == '\n')
	{
		ValidStartLine(_start_line);
		if (_uri.GetHost().empty() && _header.GetValue("Host").empty())
			_scode = BAD_REQUEST;
		href.erase(0,2);
		_step = BODY;
	}
	if (_step == BODY)
		ParseBody(href, config);
	else
		_step = END;
}
