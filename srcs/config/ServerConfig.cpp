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
	this->_methods_allowed = src._methods_allowed;
	this->_autoindex = src._autoindex;
	this->_body_size = src._body_size;
	this->_root = src._root;
	this->_index = src._index;
	this->_error_pages = src._error_pages;
	return (*this);
}

BlockConfig & ServerConfig::getBlockConfigFromURI(Uri const &uri)
{
	std::vector<std::string> path = uri.GetPath();
	std::string str_path;
	std::string location_name("");
	
	for (std::vector<std::string>::iterator p_it = path.begin();p_it != path.end();p_it++)
	{
		str_path = str_path + (*p_it);
		for (std::map<std::string, LocationConfig>::const_iterator it = this->_locations.begin();it != this->_locations.end();it++)
			if (str_path.compare((*it).first) == 0)
				location_name = (*it).first;
	}
	if (location_name.empty())
		return (*this);
	return (this->_locations[location_name]);
}

BlockConfig const &	ServerConfig::getBlockConfigFromURI(Uri const &uri) const
{
	std::vector<std::string> path = uri.GetPath();
	std::string str_path;
	std::string location_name("");
	
	for (std::vector<std::string>::iterator p_it = path.begin();p_it != path.end();p_it++)
	{
		str_path = str_path + (*p_it);
		for (std::map<std::string, LocationConfig>::const_iterator it = this->_locations.begin();it != this->_locations.end();it++)
			if (str_path.compare((*it).first) == 0)
				location_name = (*it).first;
	}
	if (location_name.empty())
		return (*this);
	return (this->_locations.find(location_name)->second);
}