#include "Request.hpp"

/*
	REQUEST HTTP
*/
Request::Request() : _start_line(""), _header(HttpHeader()), _body("")
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

// void		Request::ParseRequest(std::string href)
// {
// }

// bool	Request::isChunked(void)
// {
// 	const std::string	chunk = ;
// }
