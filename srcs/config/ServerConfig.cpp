#include "../webserv.hpp"

ServerConfig::ServerConfig() : _port(8080), _server_name(""), _locations() {}

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
	this->_locations = src._locations;
	this->_server_name = src._server_name;
	
	this->_values_set = src._values_set;
	this->_autoindex = src._autoindex;
	this->_body_size = src._body_size;
	this->_limited_body_size = src._limited_body_size;
	this->_root = src._root;
	this->_index = src._index;
	this->_error_pages = src._error_pages;
	return (*this);
}

