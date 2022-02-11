#include "../webserv.hpp"

/*
	REQUEST HTTP
*/
Request::Request() : _step(START), _start_line(""), _header(Header()), _body(""),  _ishost(false)
{

}

Request::Request(const Request& cop) : _start_line(cop._start_line), _header(cop._header), _body(cop._body)
{}

Request&	Request::operator=(const Request& cop)
{
	_start_line = cop._start_line;
	_header = cop._header;
	_body = cop._body;
	return *this;
}

Request::~Request()
{
	
}

/*
void		Request::ParseRequest(std::string href)
{
	//std::string start = this->_re
	std::string start = href;
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

	Maybe supp this
//bool		Request::isValidHeader(void) const
//{
	// _header;
//	return true;
//}

	@brief: Parse the Header Request step by step or in once time
	@param: href = brut header by Client
*/

void		Request::ParseRequest(std::string href)
{
	std::size_t pos;
	//	Get the Request line
	if (_step == START) {
		pos = href.find("\n");
		_start_line = href.substr(0, pos - 1);
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
	if (_uri.GetHost().empty() && _header.GetValue("Host").empty())
		_scode = BAD_REQUEST;
	_step = BODY;
	// isValidHeader(head);
	if (_step == BODY && this->GetMethod() == "POST")
		ParseBody(href);
	else
		_step = END;
}

//	GET / HTTP/1.1
//	Host: 149.202.43.113:8082
//	Connection: keep-alive
//	Cache-Control: max-age=0
//	Upgrade-Insecure-Requests: 1
//	User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36
//	Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
//	Accept-Encoding: gzip, deflate
//	Accept-Language: fr-FR,fr;en-US;

// bool	Request::isChunked(void)
// {
// 	const std::string	chunk = ;
// }

void	Request::ParseBody(std::string body)
{
	if (_header.GetValue("Transfer-Encoding").find("chunked") == 6)
	{
		; // to do
	}
	else if (_header.GetHeader()["Content-Length"].c_str())
	{
		std::size_t	lenght = strtol(_header.GetValue("Content-Length").c_str(), NULL, 10);
		if (lenght == body.size())
		{
			_scode = OK;
			_body = body;
		}
		// else if (_max_body_size > body.size())
		// {
		// 	_scode = REQUEST_ENTITY_TOO_LARGE;
		// }
	}
	else if (!body.empty())
		_scode = LENGTH_REQUIRED;
	_step = END;
}

// void	Request::ParseBodyChunked(std::string& href)
// {
// }
