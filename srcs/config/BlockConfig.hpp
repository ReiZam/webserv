
#ifndef BLOCK_CONFIG_HPP
# define BLOCK_CONFIG_HPP

# include "../webserv.hpp"

class BlockConfig
{
	public:
		BlockConfig();
		BlockConfig(BlockConfig const &src);
		virtual ~BlockConfig();


		BlockConfig &					operator=(BlockConfig const &src);
		
		bool							isAutoIndex() const
		{
			return (this->_autoindex);
		}

		int								getBodySize() const
		{
			return (this->_body_size);
		}

		bool							isValueSet(std::string key)
		{
			return (this->_values_set[key] == true);
		}

		bool							isLimitedBodySize() const
		{
			return (this->_limited_body_size);
		}

		std::string						getRoot() const
		{
			return (this->_root);
		}

		std::string						getIndex() const
		{
			return (this->_index);
		}

		std::map<std::string, bool> & 	getMethodsAllowed()
		{
			return (this->_methods_allowed);
		}

		void							addErrorPage(int error_code, std::string path)
		{
			this->_error_pages[error_code] = path;
		}

		void							setRoot(std::string root)
		{
			this->_root = root;
		}

		void							setIndex(std::string index)
		{
			this->_index = index;
		}

		void							setValue(std::string key, bool set)
		{
			this->_values_set[key] = set;
		}

		void							setBodySize(int body_size)
		{
			this->_body_size = body_size;
			this->_limited_body_size = true;
		}

		void							setAutoIndex(bool autoindex)
		{
			this->_autoindex = autoindex;
		}
	protected:
		std::map<std::string, bool>				_values_set;
		std::map<std::string, bool>				_methods_allowed;
		bool 									_autoindex;
		int										_body_size;
		bool									_limited_body_size;
		std::string								_root;
		std::string								_index;
		std::map<int, std::string>				_error_pages;
};

#endif