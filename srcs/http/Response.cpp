#include "Response.hpp"

Response::Response()
{

}

Response::~Response()
{

}

Response::Response(Response const &src)
{
    this->_response_body = src._response_body;
    this->_response_cgi_body = src._response_cgi_body;
    this->_response_cgi_hearder = src._response_cgi_hearder;
    this->_response_header = src._response_header;
    this->_response_line = src._response_line;
    this->_response_status = src._response_status;
    this->_response_status_table = src._response_status_table;
}


Response    &   Response::operator=(Response const &src)
{
    this->_response_body = src._response_body;
    this->_response_cgi_body = src._response_cgi_body;
    this->_response_cgi_hearder = src._response_cgi_hearder;
    this->_response_header = src._response_header;
    this->_response_line = src._response_line;
    this->_response_status = src._response_status;
    this->_response_status_table = src._response_status_table;

    return *this;
}

void    Response::clear_response(void)
{
    this->_response_line.clear();
    this->_response_header.clear();
    this->_response_body.clear();
}

void    Response::status_response(void)
{
    this->_response_status_table.insert(std::make_pair(100, "Continue"));
    this->_response_status_table.insert(std::make_pair(101, "Switching Protocols"));
    this->_response_status_table.insert(std::make_pair(200, "OK"));
    this->_response_status_table.insert(std::make_pair(201, "CREATED"));    
    this->_response_status_table.insert(std::make_pair(202, "Accepted"));
    this->_response_status_table.insert(std::make_pair(203, "Non-Authoritative Information"));
    this->_response_status_table.insert(std::make_pair(204, "NO_Content"));
    this->_response_status_table.insert(std::make_pair(205, "Reset Content"));
    this->_response_status_table.insert(std::make_pair(206, "Partial Content"));
    this->_response_status_table.insert(std::make_pair(300, "Multiple Choices"));
    this->_response_status_table.insert(std::make_pair(301, "Moved Permanently"));
    this->_response_status_table.insert(std::make_pair(302, "Found"));
    this->_response_status_table.insert(std::make_pair(303, "See Other"));
    this->_response_status_table.insert(std::make_pair(304, "Not Modified"));
    this->_response_status_table.insert(std::make_pair(305, "Use Proxy"));
    this->_response_status_table.insert(std::make_pair(307, "Temporary Redirect"));
    this->_response_status_table.insert(std::make_pair(400, "Bad Request"));
    this->_response_status_table.insert(std::make_pair(401, "Unauthorized"));
    this->_response_status_table.insert(std::make_pair(402, "Payment Required"));
    this->_response_status_table.insert(std::make_pair(403, "Forbidden"));
    this->_response_status_table.insert(std::make_pair(404, "Not Found"));
    this->_response_status_table.insert(std::make_pair(405, "Not Allowed Method"));
    this->_response_status_table.insert(std::make_pair(406, "Not Acceptable"));
    this->_response_status_table.insert(std::make_pair(407, "Proxy Authentification Required"));
    this->_response_status_table.insert(std::make_pair(408, "Request Time-out"));
    this->_response_status_table.insert(std::make_pair(409, "Conflict"));
    this->_response_status_table.insert(std::make_pair(411, "Length Required"));
    this->_response_status_table.insert(std::make_pair(412, "Precondition Failed"));
    this->_response_status_table.insert(std::make_pair(413, "Payload Too Large"));
    this->_response_status_table.insert(std::make_pair(414, "URI Too Long"));
    this->_response_status_table.insert(std::make_pair(415, "Unsupported Media Type"));
    this->_response_status_table.insert(std::make_pair(416, "Range Not Satisfiable"));
    this->_response_status_table.insert(std::make_pair(417, "Expectation Failed"));
    this->_response_status_table.insert(std::make_pair(426, "Upgrade Required"));
    this->_response_status_table.insert(std::make_pair(500, "Internal Server Error"));
    this->_response_status_table.insert(std::make_pair(501, "Not Implemented"));
    this->_response_status_table.insert(std::make_pair(502, "Bad Gateway"));
    this->_response_status_table.insert(std::make_pair(503, "Service Unaivailable"));
    this->_response_status_table.insert(std::make_pair(504, "Gateway Timeout"));
    this->_response_status_table.insert(std::make_pair(505, "HTPP Version not Supported"));
}