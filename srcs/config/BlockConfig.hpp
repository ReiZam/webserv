#ifndef BLOCK_CONFIG_HPP
# define BLOCK_CONFIG_HPP

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

		bool							isValueSet(std::string key) const
		{
			return (this->_values_set.find(key)->second == true);
		}

		std::string						getRoot() const
		{
			return (this->_root);
		}

		std::string						getIndex() const
		{
			return (this->_index);
		}

		std::map<std::string, bool> &	getMethodsAllowed()
		{
			return (this->_methods_allowed);
		}

		std::map<std::string, bool> 	getMethodsAllowed() const
		{
			return (this->_methods_allowed);
		}

		std::map<std::string, std::string> &	getCgi()
		{
			return (this->_cgi);
		}

		std::map<std::string, std::string>		getCgi() const
		{
			return (this->_cgi);
		}

		std::string			GetCgiExtension(const std::string& extension) { return this->_cgi[extension];}

		bool							isCgiExtension(std::string extension) const
		{
			return (!this->_cgi.find(extension)->second.empty());
		}

		std::map<int, std::string>	getErrorPages() const
		{
			return (this->_error_pages);
		}

		void							addErrorPage(int error_code, std::string path)
		{
			this->_error_pages[error_code] = path;
		}

		void							addCgiExtension(const std::string extension, const std::string path)
		{
			this->_cgi[extension] = path;
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
		}

		void							setAutoIndex(bool autoindex)
		{
			this->_autoindex = autoindex;
		}

		void							setMethodsAllowed(std::map<std::string, bool> methods_allowed)
		{
			this->_methods_allowed = methods_allowed;
		}

		void							setErrorPages(std::map<int, std::string> error_pages)
		{
			this->_error_pages = error_pages;
		}

		void							setCgiExtensions(std::map<std::string, std::string> cgi)
		{
			this->_cgi = cgi;
		}

	protected:
		std::map<std::string, bool>				_values_set;
		std::map<std::string, bool>				_methods_allowed;
		bool 									_autoindex;
		int										_body_size;
		std::string								_root;
		std::string								_index;
		std::map<int, std::string>				_error_pages;
		std::map<std::string, std::string>		_cgi;
};

#endif
