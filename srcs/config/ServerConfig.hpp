#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

#include "../webserv.hpp"

class Uri;

class ServerConfig : public BlockConfig
{
	public:
		ServerConfig();
		ServerConfig(ServerConfig const &src);
		virtual ~ServerConfig();

		ServerConfig &				operator=(ServerConfig const &src);
		LocationConfig &			getLocationConfigFromURI(Uri const &uri);

		int							getPort() const
		{
			return (this->_port);
		}

		in_addr						getAddress() const
		{
			return (this->_address);
		}

		std::map<std::string, LocationConfig>	getLocations() const
		{
			return (this->_locations);
		}

		std::string					getHost() const
		{
			return (this->_host);
		}
		
		std::string					getServerName() const
		{
			return (this->_server_name);
		}

		void						setHost(std::string host, std::string address, int port)
		{
			this->_host = host;
			this->_address.s_addr = inet_addr(address.c_str());
			this->_port = port;
		}
		
		void						setServerName(std::string const &server_name)
		{
			this->_server_name = server_name;
		}
		
		void						addLocationConfig(LocationConfig &location_config)
		{
			this->_locations[location_config.getLocationName()] = location_config;
		}
	private:
		int										_port;
		std::string								_host;
		struct in_addr  						_address;
		std::string								_server_name;
		std::map<std::string, LocationConfig>	_locations;
};

#endif