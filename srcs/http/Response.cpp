#include "../webserv.hpp"

Response::Response()
{   
    gen_status_code(this->_response_code);    
}

Response::~Response()
{

}

Response::Response(Response const &cop)
{
    *this = cop;
}

Response&   Response::operator=(Response const &cop)
{
   //§ *this = cop;
    this->_response_code = cop._response_code;
    this->_start_line = cop._start_line;
    return *this;
}