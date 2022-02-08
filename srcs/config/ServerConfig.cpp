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

LocationConfig & ServerConfig::getLocationConfigFromURI(Uri const &uri)
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
		throw Config::ConfigException("Config Getter", "No Location Config found");
	return (this->_locations[location_name]);
}