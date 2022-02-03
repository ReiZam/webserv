#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP 

#include "../webserv.hpp"

class LocationConfig
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

		std::string					getRoot() const
		{
			return (this->_root);
		}

		std::string					getIndex() const
		{
			return (this->_index);
		}

		bool						isAutoIndex() const
		{
			return (this->_autoindex);
		}

		bool						isSet(std::string key)
		{
			return (this->_values_set[key] == true);
		}

		std::map<std::string, bool> getValuesSet()
		{
			return (this->_values_set);
		}

		void						setAutoIndex(bool autoindex)
		{
			this->_autoindex = autoindex;
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
		std::string								_location_name;
		std::map<std::string, bool>				_values_set;

		bool 									_autoindex;
		int										_body_size;
		bool									_limited_body_size;
		std::string								_root;
		std::string								_index;
		std::map<int, std::string>				_error_pages;
};

#endif