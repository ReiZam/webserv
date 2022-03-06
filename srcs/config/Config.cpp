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

std::vector<ServerConfig> const &	Config::getServersConfig() const
{
	return (this->_servers_config);
}

bool	Config::is_file_valid() const
{
	if (this->_file_name.empty() || check_ext_path(this->_file_name, ".conf"))
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
		if ((*it).getTokenType() == LeftCurly)
			ratio--;
		++it;
	}
	return (ratio == 0);
}

bool	Config::check_server_config(ServerConfig &config)
{
	if (!config.isValueSet("listen"))
		return (false);

	std::map<std::string, LocationConfig> &locations = config.getLocations();

	for (std::map<std::string, LocationConfig>::iterator it = locations.begin();it != locations.end();it++)
	{
		std::string name = (*it).first;
		LocationConfig &location_config = (*it).second;

		if (!location_config.isValueSet("root"))
			location_config.setRoot(config.getRoot());
		if (!location_config.isValueSet("index"))
			location_config.setIndex(config.getIndex());
		if (!location_config.isValueSet("limit_body_size"))
			location_config.setBodySize(config.getBodySize());
		if (!location_config.isValueSet("autoindex"))
			location_config.setAutoIndex(config.isAutoIndex());
		if (!location_config.isValueSet("allow_methods"))
			location_config.setMethodsAllowed(config.getMethodsAllowed());
		if (!location_config.isValueSet("error_pages"))
			location_config.setErrorPages(config.getErrorPages());
		if (!location_config.isValueSet("cgi_extensions"))
			location_config.setCgiExtensions(config.getCgi());
		if (!location_config.isValueSet("file_upload_directory"))
			location_config.setFileUploadDirectory(config.getFileUploadDirectory());
	}
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

	if (address.compare(0,10,"localhost:") == 0)
		address.replace(0, 9, "");
	std::size_t	pos = address.find(":");
	if (address.find("::") == std::string::npos)
		if (pos == 0 || pos == std::string::npos)
		{
			for (std::string::const_iterator it=address.begin(); it!=address.end(); ++it)
			{
				if (*it == ':')
					++it;
				if (!std::isdigit(*it))
					throw ConfigException("Configuration parse failed", "Invalid host value");
			}
			port = (pos == 0) ? strtol(address.c_str()+1, NULL, 10) : strtol(address.c_str(), NULL, 10);
			std::string listen = (pos == 0) ? address : ":" + address;
			if (port > 65535 || port < 0)
				throw ConfigException("Configuration parse failed", "Invalid host:port value");
			config.setHost(LOCALHOST + listen, LOCALHOST, port);
			return ;
		}
	if (sscanf(address.c_str(),"%d.%d.%d.%d:%d", &a, &b, &c, &d, &port) != 5)
		throw ConfigException("Configuration parse failed", "Invalid host format");
	if (a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255 || port < 0 || port > 65535)
		throw ConfigException("Configuration parse failed", "Invalid host value");
	config.setHost(address, address.substr(0, address.find(":")), port);
}

void	Config::parse_buffer_size(std::string value, BlockConfig &config)
{
	int i = -1;
	
	while (value[++i])
		if (value[i] < '0' || value[i] > '9')
			throw ConfigException("Configuration parse failed", "Invalid body size format");
	config.setBodySize(static_cast<int>(std::atol(value.c_str())));
}

void	Config::parse_autoindex(std::string value, BlockConfig &config)
{
	if (value.compare("on") != 0 && value.compare("off") != 0)
		throw ConfigException("Configuration parse failed", "Invalid auto index format");
	config.setAutoIndex(value.compare("on") == 0);
}

void	Config::parse_location_config_line(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, LocationConfig &config)
{
	if (!this->check_config_line(it, ite))
		throw ConfigException("Configuration parse failed", "Invalid location config line");
	if (!this->parse_block_config_line(it, config))
		throw ConfigException("Configuration parse failed", "Invalid server config element");
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

void	Config::parse_methods_allowed(std::string value, BlockConfig &config)
{
	std::map<std::string, bool> &methods_allowed = config.getMethodsAllowed();
	size_t						current_pos = 0;

	methods_allowed["GET"] = false;
	methods_allowed["POST"] = false;
	methods_allowed["DELETE"] = false;
	while (current_pos != std::string::npos)
	{
		current_pos = value.find(",");
		std::string method = value.substr(0, current_pos);

		if (method.compare("GET") != 0 && method.compare("POST") != 0 && method.compare("DELETE") != 0)
			throw ConfigException("Configuration parse failed", "Invalid method");
		if (methods_allowed[method] == true)
			throw ConfigException("Configuration parse failed", "Method set multiple times");
		methods_allowed[method] = true;
		value = value.substr(current_pos + 1);
	}
}

void	Config::parse_cgi_extensions(std::string value, BlockConfig &config)
{
	std::map<std::string, std::string> &cgi_extensions = config.getCgi();
	size_t current_pos = value.find(",");

	if (current_pos == std::string::npos || value.find(",") != value.rfind(","))
		throw ConfigException("Configuration parse failed", "CGI extension set multiple times");
	std::string	entry = value.substr(0, current_pos);
	if (!cgi_extensions[entry].empty())
		throw ConfigException("Configuration parse failed", "CGI extension set multiple times");
	if (entry.find(".") != 0 || entry.rfind(".") != entry.find("."))
		throw ConfigException("Configuration parse failed", "Invalid CGI extension");
	value = value.substr(current_pos + 1);
	if (value.empty() || !exist_file(value))
		throw ConfigException("Configuration parse failed", "Invalid CGI extension");
	cgi_extensions[entry] = value;
	
}

void	Config::parse_error_page(std::string value, BlockConfig &config)
{
	int error_code = static_cast<int>(std::atol(value.c_str()));

	if (value.find(":") == (unsigned long)-1 || value.find(":") + 1 >= value.size())
		throw ConfigException("Configuration parse failed", "Invalid error page format");
	if (error_code < 100 || error_code > 599)
		throw ConfigException("Configuration parse failed", "Invalid error page code");
	config.addErrorPage(error_code, value.substr(value.find(":") + 1));
}

void	Config::parse_file_upload_directory(std::string value, BlockConfig &config)
{
	if (!exist_directory(value))
		throw ConfigException("Configuration parse failed", "Invalid file upload directory");
	config.setFileUploadDirectory(value);
}

bool	Config::parse_block_config_line(std::vector<ConfigLexer::Token>::iterator &it, BlockConfig &config)
{
	if ((*it).getString().compare("limit_body_size") == 0)
	{
		if (config.isValueSet("limit_body_size") == true)
			throw ConfigException("Configuration parse failed", "Body size limit already set");
		this->parse_buffer_size((*(++it)).getString(), config);
		config.setValue("limit_body_size", true);
	}
	else if ((*it).getString().compare("autoindex") == 0)
	{
		if (config.isValueSet("autoindex") == true)
			throw ConfigException("Configuration parse failed", "Auto index already set");
		this->parse_autoindex((*(++it)).getString(), config);
		config.setValue("autoindex", true);
	}
	else if ((*it).getString().compare("error_page") == 0)
	{
		this->parse_error_page((*(++it)).getString(), config);
		config.setValue("error_page", true);
	}
	else if ((*it).getString().compare("root") == 0)
	{
		if (config.isValueSet("root") == true)
			throw ConfigException("Configuration parse failed", "Root already set");
		config.setRoot((*(++it)).getString());
		config.setValue("root", true);
	}
	else if ((*it).getString().compare("index") == 0)
	{
		if (config.isValueSet("index") == true)
			throw ConfigException("Configuration parse failed", "Index already set");
		config.setIndex((*(++it)).getString());
		config.setValue("index", true);
	}
	else if ((*it).getString().compare("allow_methods") == 0)
	{
		if (config.isValueSet("allow_methods") == true)
			throw ConfigException("Configuration parse failed", "Methods allowed already set");
		this->parse_methods_allowed((*(++it)).getString(), config);
		config.setValue("allow_methods", true);
	}
	else if ((*it).getString().compare("cgi") == 0)
	{
		this->parse_cgi_extensions((*(++it)).getString(), config);
		config.setValue("cgi", true);
	}
	else if ((*it).getString().compare("file_upload_directory") == 0)
	{
		if (config.isValueSet("file_upload_directory") == true)
			throw ConfigException("Configuration parse failed", "File upload folder already set");
		this->parse_file_upload_directory((*(++it)).getString(), config);
		config.setValue("file_upload_directory", true);
	}
	else
		return (false);
	return (true);
}

void	Config::parse_server_config_line(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, ServerConfig &config)
{
	if (!this->check_config_line(it, ite))
		throw ConfigException("Configuration parse failed", "Invalid server config line");
	if ((*it).getString().compare("listen") == 0)
	{
		if (config.isValueSet("listen") == true)
			throw ConfigException("Configuration parse failed", "Server host already set");
		config.setValue("listen", true);
		this->parse_ipv4_address((*(++it)).getString(), config);
	}
	else if ((*it).getString().compare("server_name") == 0)
	{
		if (config.isValueSet("server_name") == true)
			throw ConfigException("Configuration parse failed", "Server name already set");
		config.setValue("server_name", true);
		config.setServerName((*(++it)).getString());
	}
	else if ((*it).getString().compare("location") == 0)
	{
		this->parse_location_config(++it, ite, config);
		return ;
	}
	else if (!this->parse_block_config_line(it, config))
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

void	Config::addServerConfig(ServerConfig const &server)
{
	for (std::vector<ServerConfig>::iterator it = this->_servers_config.begin();it != this->_servers_config.end();it++)
		if ((*it).getHost().compare(server.getHost()) == 0 || (*it).getServerName().compare(server.getServerName()) == 0) return ;
	this->_servers_config.push_back(server);
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
		this->addServerConfig(tmp);
	}
	std::cout << "[Config] Config parse: success!" << std::endl;
}
