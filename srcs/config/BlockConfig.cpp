#include "../webserv.hpp"

BlockConfig::BlockConfig() : _values_set(), _autoindex(true), _body_size(0), _limited_body_size(false), _root("html"), _index("index.html"), _error_pages() {}

BlockConfig::BlockConfig(BlockConfig const &src)
{
	*this = src;
}

BlockConfig::~BlockConfig() {}

BlockConfig &	BlockConfig::operator=(BlockConfig const &src)
{
	this->_values_set = src._values_set;
	this->_autoindex = src._autoindex;
	this->_body_size = src._body_size;
	this->_limited_body_size = src._limited_body_size;
	this->_root = src._root;
	this->_index = src._index;
	this->_error_pages = src._error_pages;
	return (*this);
}