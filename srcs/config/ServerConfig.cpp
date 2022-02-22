#include "../webserv.hpp"

ServerConfig::ServerConfig() : _port(8080), _server_name(""), _locations(), _media_types()
{
	this->registerMediaTypes();
}

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
	this->_cgi_extensions = src._cgi_extensions;
	this->_locations = src._locations;
	this->_server_name = src._server_name;
	this->_values_set = src._values_set;
	this->_methods_allowed = src._methods_allowed;
	this->_autoindex = src._autoindex;
	this->_body_size = src._body_size;
	this->_root = src._root;
	this->_index = src._index;
	this->_error_pages = src._error_pages;
	this->_media_types = src._media_types;
	return (*this);
}

void	ServerConfig::registerMediaTypes()
{
	this->_media_types[".avi"] = "video/x-msvideo";
	this->_media_types[".bin"] = "application/octet-stream";
	this->_media_types[".bmp"] = "image/bmp";
	this->_media_types[".css"] = "text/css";
	this->_media_types[".csv"] = "text/csv";
	this->_media_types[".gif"] = "image/gif";
	this->_media_types[".htm"] = "text/html";
	this->_media_types[".html"] = "text/html";
	this->_media_types[".ico"] = "image/x-icon";
	this->_media_types[".jpg"] = "image/jpeg";
	this->_media_types[".jpeg"] = "image/jpeg";
	this->_media_types[".js"] = "application/javascript";
	this->_media_types[".json"] = "application/json";
	this->_media_types[".mpeg"] = "video/mpeg";
	this->_media_types[".otf"] = "font/otf";
	this->_media_types[".png"] = "image/png";
	this->_media_types[".pdf"] = "application/pdf";
	this->_media_types[".rar"] = "application/x-rar-compressed";
	this->_media_types[".svg"] = "image/svg+xml";
	this->_media_types[".tar"] = "application/x-tar";
	this->_media_types[".ts"] = "application/typescript";
	this->_media_types[".ttf"] = "font/ttf";
	this->_media_types[".wav"] = "audio/x-wav";
	this->_media_types[".zip"] = "application/zip";
	this->_media_types[".xml"] = "application/xml";
	this->_media_types[".webp"] = "image/wbep";
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

LocationConfig const &	ServerConfig::getLocationConfigFromURI(Uri const &uri) const
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
		throw Config::ConfigException("Configuration Getter", "Location Not Found");
	return (this->_locations.find(location_name)->second);
}

std::string	ServerConfig::getMediaType(std::string const &path, std::string const &default_value) const
{
	std::string ext;

	if (path.rfind(".") != std::string::npos)
		ext = path.substr(path.rfind("."), path.size());
	if (this->_media_types.find(ext) != this->_media_types.end())
		return ((*(this->_media_types.find(ext))).second);
	// return ("application/octet-stream");
	return (default_value);
}