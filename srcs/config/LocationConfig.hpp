#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP 

#include <webserv.hpp>

//class BlockConfig;

class LocationConfig : public BlockConfig
{
	public:
		LocationConfig();
		LocationConfig(LocationConfig const &src);
		LocationConfig(std::string location_name);
		virtual ~LocationConfig();

		LocationConfig &			operator=(LocationConfig const &src);

		std::string					getLocationName() const
		{
			return (this->_location_name);
		}
	private:
		std::string								_location_name;
};

#endif
