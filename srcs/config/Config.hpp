#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../webserv.hpp"

class Config
{
    public:
        Config();
        Config(std::string file);
        Config(Config const &src);
        virtual ~Config();
    
        Config	&									operator=(Config const &config);

		std::string	const &							getConfigurationFileName() const;
		std::ifstream const &						getConfigurationFile() const;
		std::map<std::string, ServerConfig> const &	getServersConfig() const;
		void										init();
		void										parse();

		class ConfigException : public std::exception
        {
            public:
                ConfigException(std::string error_category, std::string error_text)
                {
					this->error = "[ConfigException] " + error_category +  ": " + error_text;
                }

                virtual ~ConfigException() throw() {};
            
                virtual const char *what() const throw(){
                    return (this->error.c_str());
                };
            private:
                std::string error;
        };
    private:
		std::string									_file_name;
		std::ifstream								_file;
		std::map<std::string, ServerConfig>			_servers_config;
		std::string									_file_content;

		bool			is_file_valid() const;

		bool				check_curly(std::vector<ConfigLexer::Token>::iterator it, std::vector<ConfigLexer::Token>::iterator const &ite);
		bool				check_server_config(ServerConfig &config);
		bool				check_config_line(std::vector<ConfigLexer::Token>::iterator it, std::vector<ConfigLexer::Token>::iterator const &ite);

		// SERVER CONFIG PARSER
		void				parse_error_page(std::string value, ServerConfig &config);
		void				parse_ipv4_address(std::string address, ServerConfig &config);
		void				parse_buffer_size(std::string value, ServerConfig &config);
		void				parse_autoindex(std::string value, ServerConfig &config);
		void				parse_server_config_line(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, ServerConfig &config);
		void				parse_server_config(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, ServerConfig &server_config);
		// LOCATION CONFIG PARSER
		void				parse_buffer_size(std::string value, LocationConfig &config);
		void				parse_error_page(std::string value, LocationConfig &config);
		void				parse_autoindex(std::string value, LocationConfig &config);
		void				parse_location_config_line(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, LocationConfig &config);
		void				parse_location_config(std::vector<ConfigLexer::Token>::iterator &it, std::vector<ConfigLexer::Token>::iterator const &ite, ServerConfig &server_config);
};

#endif