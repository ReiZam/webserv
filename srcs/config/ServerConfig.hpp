#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

#include "../webserv.hpp"

class ServerConfig
{
	public:
		ServerConfig();
		ServerConfig(ServerConfig const &src);
		virtual ~ServerConfig();

		ServerConfig &				operator=(ServerConfig const &src);

		int							getPort() const
		{
			return (this->_port);
		}

		bool						isAutoIndex() const
		{
			return (this->_autoindex);
		}

		int							getBodySize() const
		{
			return (this->_body_size);
		}

		in_addr						getAddress() const
		{
			return (this->_address);
		}

		std::string					getRoot() const
		{
			return (this->_root);
		}

		std::string					getIndex() const
		{
			return (this->_index);
		}

		std::map<std::string, LocationConfig>	getLocations() const
		{
			return (this->_locations);
		}

		bool						isSet(std::string key)
		{
			return (this->_values_set[key] == true);
		}

		bool						isLimitedBodySize() const
		{
			return (this->_limited_body_size);
		}

		std::string					getHost() const
		{
			return (this->_host);
		}
		
		std::string					getServerName() const
		{
			return (this->_server_name);
		}

		std::map<std::string, bool> &getValuesSet()
		{
			return (this->_values_set);
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
		
		void						setValue(std::string key, bool set)
		{
			this->_values_set[key] = set;
		}

		void						setBodySize(int body_size)
		{
			this->_body_size = body_size;
			this->_limited_body_size = true;
		}

		void						setAutoIndex(bool autoindex)
		{
			this->_autoindex = autoindex;
		}

		void						addLocationConfig(LocationConfig &location_config)
		{
			this->_locations[location_config.getLocationName()] = location_config;
		}

		void						addErrorPage(int error_code, std::string path)
		{
			this->_error_pages[error_code] = path;
		}

		void						setRoot(std::string root)
		{
			this->_root = root;
		}

		void						setIndex(std::string index)
		{
			this->_index = index;
		}
	private:
		int										_port;
		std::string								_host;
		struct in_addr  						_address;
		std::string								_server_name;
		std::map<std::string, LocationConfig>	_locations;
		std::map<std::string, bool>				_values_set;

		bool 									_autoindex;
		int										_body_size;
		bool									_limited_body_size;
		std::string								_root;
		std::string								_index;
		std::map<int, std::string>				_error_pages;
};

#endif