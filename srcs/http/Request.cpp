#include "Request.hpp"

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

void		Request::ParseRequest(std::string href)
{
	std::string key;
	std::string value;

	std::size_t	pos = href.find("\n");
	_start_line = href.substr(0, pos);
	href.erase(0, pos);
	while ((pos = href.find(":")) != std::string::npos)
	{
		
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
