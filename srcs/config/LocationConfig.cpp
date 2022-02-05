#include "../webserv.hpp"

LocationConfig::LocationConfig() : _location_name("") {}

LocationConfig::LocationConfig(std::string location_name) : _location_name(location_name) {}

LocationConfig::LocationConfig(LocationConfig const &src)
{
	*this = src;
}

LocationConfig::~LocationConfig() {}

LocationConfig &	LocationConfig::operator=(LocationConfig const &src)
{
	this->_location_name = src._location_name;
	this->_values_set = src._values_set;
	this->_autoindex = src._autoindex;
	this->_body_size = src._body_size;
	this->_limited_body_size = src._limited_body_size;
	this->_root = src._root;
	this->_index = src._index;
	this->_error_pages = src._error_pages;
	return (*this);
}