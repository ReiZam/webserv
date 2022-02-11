#include "../webserv.hpp"

Response::Response()
{
}

Response::~Response()
{
}

Response::Response(Response const &src)
{
}


Response&   Response::operator=(Response const &src)
{

    return *this;
}

// void    Response::clear_response(void)
// {
    // this->_response_line.clear();
    // this->_response_header.clear();
    // this->_response_body.clear();
// }