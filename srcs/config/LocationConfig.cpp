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
	this->_methods_allowed = src._methods_allowed;
	this->_autoindex = src._autoindex;
	this->_body_size = src._body_size;
	this->_cgi = src._cgi;
	this->_root = src._root;
	this->_index = src._index;
	this->_error_pages = src._error_pages;
	this->_file_upload_directory = src._file_upload_directory;
	return (*this);
}
