#include "../webserv.hpp"

ServerConfig::ServerConfig() : _port(8080), _server_name(""), _locations(), _values_set(), _limited_body_size(false), _root("html"), _index("index.html"), _error_pages() {}

ServerConfig::ServerConfig(ServerConfig const &src)
{
	*this = src;
}

ServerConfig::~ServerConfig() {}

ServerConfig &	ServerConfig::operator=(ServerConfig const &src)
{
	this->_host = src._host;
	this->_address = src._address;
	this->_port = src._port;
	this->_autoindex = src._autoindex;
	this->_body_size = src._body_size;
	this->_limited_body_size = src._limited_body_size;
	this->_locations = src._locations;
	this->_server_name = src._server_name;
	return (*this);
}

