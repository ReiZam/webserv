#include "../webserv.hpp"

LocationConfig::LocationConfig() : _location_name(""), _values_set(), _autoindex(true), _root("html"), _index("index.html") {}

LocationConfig::LocationConfig(std::string location_name) : _location_name(location_name), _autoindex(true), _root("html"), _index("index.html") {}

LocationConfig::LocationConfig(LocationConfig const &src)
{
	*this = src;
}

LocationConfig::~LocationConfig() {}

LocationConfig &	LocationConfig::operator=(LocationConfig const &src)
{
	this->_location_name = src._location_name;
	return (*this);
}