#include "../webserv.hpp"

Response::Response() : _start_line(), _response_code(200) {}

Response::~Response() {}

Response::Response(Response const &cop)
{
    *this = cop;
}

Response&   Response::operator=(Response const &cop)
{
    this->_response_code = cop._response_code;
    this->_start_line = cop._start_line;
    return *this;
}