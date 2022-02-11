#include "../webserv.hpp"

/*
	REQUEST HTTP
*/
Request::Request() : _start_line(""), _header(Header()), _body("")
{}

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
{}

/*
	Utils for Request
*/
bool	Request::ValidStartLine(void)
{
	std::string start(_start_line);
	std::string method = start.substr(0, start.find(" "));
	if (method.length() == 0 || (method != "GET" && method != "HEAD" && method != "POST"
		&& method != "PUT" && method != "DELETE" && method != "CONNECT"
		&& method != "OPTIONS" && method != "TRACE" && method != "PATCH"))
		return false;
	start.erase(0, start.find(" ")+1);
	if (start.substr(0, start.find(" ")).length() == 0)
		return false;
	//	uri is parse in her class
	start.erase(0, start.find(" ")+1);
	if (start.length() == 0 || (start != "HTTP/1.0" && start != "HTTP/1.1"))
		return false;
	start.clear();
	return true;
}


void		Request::ParseRequest(std::string href)
{
	std::string key;
	std::string value;

	std::size_t	pos = href.find("\n");
	_start_line = href.substr(0, pos);
	href.erase(0, pos);
	while ((pos = href.find(":")) != std::string::npos)
	{
		key = href.substr(0, pos);
	}
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
