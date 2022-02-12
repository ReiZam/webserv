#include "../webserv.hpp"

BlockConfig::BlockConfig() : _values_set(), _methods_allowed(), _autoindex(true), _body_size(1000000), _root("/html"), _index("/index.html"), _error_pages()
{
	this->_methods_allowed["GET"] = true;
	this->_methods_allowed["POST"] = true;
	this->_methods_allowed["DELETE"] = true;
}

BlockConfig::BlockConfig(BlockConfig const &src)
{
	*this = src;
}

BlockConfig::~BlockConfig() {}

BlockConfig &	BlockConfig::operator=(BlockConfig const &src)
{
	this->_values_set = src._values_set;
	this->_methods_allowed = src._methods_allowed;
	this->_autoindex = src._autoindex;
	this->_body_size = src._body_size;
	this->_root = src._root;
	this->_index = src._index;
	this->_error_pages = src._error_pages;
	return (*this);
}