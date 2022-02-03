#include "../webserv.hpp"

Config::Config() : _file_name("default.conf"), _file(this->_file_name.c_str()), _servers_config() {}

Config::Config(std::string file) : _file_name(file), _file(this->_file_name.c_str()),  _servers_config() {}

Config::Config(Config const &src)
{
	*this = src;
}

Config::~Config()
{
	if (this->_file && this->_file.is_open())
		this->_file.close();
}

Config	&	Config::operator=(Config const &src)
{
	this->_file_name = src.getConfigurationFileName();
	this->_servers_config = src.getServersConfig();
	return (*this);
}

std::string	const &	Config::getConfigurationFileName() const
{
	return (this->_file_name);
}

std::ifstream const &	Config::getConfigurationFile() const
{
	return (this->_file);
}

std::map<std::string, ServerConfig> const &	Config::getServersConfig() const
{
	return (this->_servers_config);
}

bool	Config::is_file_valid() const
{
	if (this->_file_name.empty() || this->_file_name.size() < 5 || this->_file_name.size() - this->_file_name.find(".conf") != 5)
		return (false);
	return (this->_file && this->_file.is_open());
}

void	Config::init()
{
	std::stringstream	content_stream;

	if (!this->is_file_valid())
		throw ConfigException("Configuration init failed", "File is invalid");
	
	content_stream << this->_file.rdbuf();
	this->_file_content = content_stream.str();

	if (this->_file_content.empty())
		throw ConfigException("Configuration init failed", "File is empty");

	this->_file.close();

	std::cout << "[Config] Config init: success!" << std::endl;
}


bool			Config::check_curly(std::vector<ConfigLexer::Token>::iterator it, std::vector<ConfigLexer::Token>::iterator const &ite)
{
	int ratio;

	if ((*it++).getTokenType() != RightCurly)
		return (false);
	ratio = 1;
	while (it != ite && ratio != 0)
	{
		if ((*it).getTokenType() == RightCurly)
			ratio++;
		else if ((*it).getTokenType() == LeftCurly)
			ratio--;
		++it;
	}
	return (ratio == 0);
}

bool	Config::check_server_config(ServerConfig &config)
{
	if (!config.isSet("server_name") || !config.isSet("listen"))
		return (false);
	return (true);
}

bool	Config::check_config_line(std::vector<ConfigLexer::Token>::iterator it, std::vector<ConfigLexer::Token>::iterator const &ite)
{
	if ((*it).getTokenType() != Word)
		return (false);
	++it;
	if (it == ite || (*it).getTokenType() != Word)
		return (false);
	++it;
	if (it == ite || ((*it).getTokenType() != Semicolon && (*it).getTokenType() != RightCurly))
		return (false);
	return (true);
}

void	Config::parse_ipv4_address(std::string address, ServerConfig &config)
{
	int a, b, c, d, port;

	if (sscanf(address.c_str(),"%d.%d.%d.%d:%d", &a, &b, &c, &d, &port) != 5)
		throw ConfigException("Configuration parse failed", "Invalid host format");
	if (a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255 || port < 0 || port > 65535)
		throw ConfigException("Configuration parse failed", "Invalid host value");
	config.setHost(address, address.substr(0, address.find(":")), port);
}

void	Config::parse_buffer_size(std::string value, ServerConfig &config)
{
	int i = -1;
	
	while (value[++i])
		if (value[i] < '0' || value[i] > '9')
			throw ConfigException("Configuration parse failed", "Invalid body size format");
	config.setBodySize(static_cast<int>(std::atol(value.c_str())));
}

void	Config::parse_buffer_size(std::string value, LocationConfig &config)
{
	int i = -1;
	
	while (value[++i])
		if (value[i] < '0' || value[i] > '9')
			throw ConfigException("Configuration parse failed", "Invalid body size format");
	config.setBodySize(static_cast<int>(std::atol(value.c_str())));
}

void	Config::parse_autoindex(std::string value, ServerConfig &config)
{
	if (value.compare("on") != 0 && value.compare("off") != 0)
		throw ConfigException("Configuration parse failed", "Invalid auto index format");
	config.setAutoIndex(value.compare("on") == 0);
}

void	Config::parse_autoindex(std::string value, LocationConfig &config)
{
	if (value.compare("on") != 0 && value.compare("off") != 0)
		throw ConfigException("Configuration parse failed", "Invalid auto index format");
	config.setAutoIndex(value.compare("on") == 0);
}

void	Config::parse_location_config_line(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, LocationConfig &config)
{
	if (!check_config_line(it, ite))
		throw ConfigException("Configuration parse failed", "Invalid location config line");
	if ((*it).getString().compare("autoindex") == 0)
	{
		if (config.isSet("autoindex") == true)
			throw ConfigException("Configuration parse failed", "Auto index already set");
		config.setValue("autoindex", true);
		this->parse_autoindex((*(++it)).getString(), config);
	}
	else if ((*it).getString().compare("limit_body_size") == 0)
	{
		if (config.isSet("limit_body_size") == true)
			throw ConfigException("Configuration parse failed", "Body size limit already set");
		config.setValue("limit_body_size", true);
		this->parse_buffer_size((*(++it)).getString(), config);
	}
	else if ((*it).getString().compare("root") == 0)
	{
		if (config.isSet("root") == true)
			throw ConfigException("Configuration parse failed", "Root already set");
		config.setRoot((*(++it)).getString());
	}
	else if ((*it).getString().compare("index") == 0)
	{
		if (config.isSet("index") == true)
			throw ConfigException("Configuration parse failed", "Index already set");
		config.setIndex((*(++it)).getString());
	}
	else if ((*it).getString().compare("error_page") == 0)
		this->parse_error_page((*(++it)).getString(), config);
	else
		throw ConfigException("Configuration parse failed", "Invalid location config element");
	it += 2;
}

void	Config::parse_location_config(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, ServerConfig &config)
{
	std::string location_name = (*(it++)).getString();
	LocationConfig location_config(location_name);
	
	if (!this->check_curly(it, ite))
		throw ConfigException("Configuration parse failed", "Location block invalid");
	++it;
	while (it != ite && (*it).getTokenType() != LeftCurly)
		parse_location_config_line(it, ite, location_config);
	++it;
	config.addLocationConfig(location_config);
}

void	Config::parse_error_page(std::string value, ServerConfig &config)
{
	int error_code = static_cast<int>(std::atol(value.c_str()));

	if (value.find(":") == (unsigned long)-1 || value.find(":") + 1 >= value.size())
		throw ConfigException("Configuration parse failed", "Invalid error page format");
	if (error_code < 100 || error_code > 599)
		throw ConfigException("Configuration parse failed", "Invalid error page code");
	config.addErrorPage(error_code, value.substr(value.find(":") + 1));
}

void	Config::parse_error_page(std::string value, LocationConfig &config)
{
	int error_code = static_cast<int>(std::atol(value.c_str()));

	if (value.find(":") == (unsigned long)-1 || value.find(":") + 1 >= value.size())
		throw ConfigException("Configuration parse failed", "Invalid error page format");
	if (error_code < 100 || error_code > 599)
		throw ConfigException("Configuration parse failed", "Invalid error page code");
	config.addErrorPage(error_code, value.substr(value.find(":") + 1));
}

void	Config::parse_server_config_line(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, ServerConfig &config)
{
	if (!this->check_config_line(it, ite))
		throw ConfigException("Configuration parse failed", "Invalid server config line");
	if ((*it).getString().compare("listen") == 0)
	{
		if (config.isSet("listen") == true)
			throw ConfigException("Configuration parse failed", "Server host already set");
		config.setValue("listen", true);
		this->parse_ipv4_address((*(++it)).getString(), config);
	}
	else if ((*it).getString().compare("server_name") == 0)
	{
		if (config.isSet("server_name") == true)
			throw ConfigException("Configuration parse failed", "Server name already set");
		config.setValue("server_name", true);
		config.setServerName((*(++it)).getString());
	}
	else if ((*it).getString().compare("limit_body_size") == 0)
	{
		if (config.isSet("limit_body_size") == true)
			throw ConfigException("Configuration parse failed", "Body size limit already set");
		config.setValue("limit_body_size", true);
		this->parse_buffer_size((*(++it)).getString(), config);
	}
	else if ((*it).getString().compare("autoindex") == 0)
	{
		if (config.isSet("autoindex") == true)
			throw ConfigException("Configuration parse failed", "Auto index already set");
		config.setValue("autoindex", true);
		this->parse_autoindex((*(++it)).getString(), config);
	}
	else if ((*it).getString().compare("error_page") == 0)
		this->parse_error_page((*(++it)).getString(), config);
	else if ((*it).getString().compare("root") == 0)
	{
		if (config.isSet("root") == true)
			throw ConfigException("Configuration parse failed", "Root already set");
		config.setRoot((*(++it)).getString());
	}
	else if ((*it).getString().compare("index") == 0)
	{
		if (config.isSet("index") == true)
			throw ConfigException("Configuration parse failed", "Index already set");
		config.setIndex((*(++it)).getString());
	}
	else if ((*it).getString().compare("location") == 0)
	{
		this->parse_location_config(++it, ite, config);
		return ;
	}
	else
		throw ConfigException("Configuration parse failed", "Invalid server config element");
	it += 2;
}

void	Config::parse_server_config(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, ServerConfig &server_config)
{
	if (!this->check_curly(it, ite))
		throw ConfigException("Configuration parse failed", "Server block invalid");
	++it;
	while (it != ite && (*it).getTokenType() != LeftCurly)
		this->parse_server_config_line(it, ite, server_config);
	++it;
	if (!this->check_server_config(server_config))
		throw ConfigException("Configuration parse failed", "Server block incomplete");
}

void	Config::parse()
{
	
	ConfigLexer lexer(this->_file_content);
	std::vector<ConfigLexer::Token>	tokens = lexer.getTokens();
	std::vector<ConfigLexer::Token>::iterator it = tokens.begin();

	
	it = tokens.begin();

	while (it != tokens.end())
	{
		if ((*it).getString().compare("server") != 0)
			throw ConfigException("Configuration parse failed", "Server block not found");
		++it;
		if (it == tokens.end())
			throw ConfigException("Configuration parse failed", "Invalid config");
		ServerConfig tmp;
		this->parse_server_config(it, tokens.end(), tmp);
		this->_servers_config[tmp.getServerName()] = tmp;
	}

	std::cout << "[Config] Config parse: success!" << std::endl;
}