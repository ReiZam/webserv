#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

#include "../webserv.hpp"

class ServerConfig : public BlockConfig
{
	public:
		ServerConfig();
		ServerConfig(ServerConfig const &src);
		virtual ~ServerConfig();

		ServerConfig &				operator=(ServerConfig const &src);
		BlockConfig &				getBlockConfigFromURI(Uri const &uri);
		BlockConfig const &			getBlockConfigFromURI(Uri const &uri) const;
		LocationConfig const &		getLocationConfigFromURI(Uri const &uri) const;
		std::string					getMediaType(std::string const &path) const;

		void	registerMediaTypes();

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
		std::map<std::string, std::string>		_media_types;
};

#endif