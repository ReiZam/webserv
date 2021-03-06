#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <webserv.hpp>

class BlockConfig;
class Client;

class	Response
{
	public:
		Response();
		Response(Response const &cop);
		virtual ~Response();
  
		Response&		operator=(const Response& cop);

		std::string	getRawHeader(void)	{ return (this->_raw_header); }
		int			getResponseCode(void) const { return (this->_response_code); }

		bool		isValidResponseCode(void) const
		{ return (this->_response_code >= 200 && this->_response_code < 300); }

		void		setResponseCode(int response_code) { this->_response_code = response_code; }
		void		setFinished(bool finished) { this->_finished = finished; }

		std::vector<unsigned char>&	getBody(void) { return (this->_body); }

		void		generateResponse(Client &client, Request &request, ServerConfig const &config);
		bool		isFinished() { return (this->_finished); }
	private:
		Header						_header;
		std::string					_raw_header;
		std::vector<unsigned char>	_body;
		int							_response_code;
		bool						_finished;

		std::string	parsePath(Request &request, ServerConfig const &config, BlockConfig const &block_config);
		void		write_body_with_file(Client &client, Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path);
		char**		generate_cgi_env(Client &client, Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path);
		void		execute_cgi(Client &client, Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path, std::string cgi_path);
		void		write_error_body(ServerConfig const &config, BlockConfig const &block_config);
		void		write_body_autoindex(std::string path);
		void		parse_cgi_response(std::string cgi_response);
		bool		write_error_page_from_errcode(ServerConfig const &config, BlockConfig const &block_config, int error_code);
		void		save_uploaded_files(std::string client_body, Request &request, BlockConfig const &block_config);
};

#endif
