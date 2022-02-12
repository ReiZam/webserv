#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../webserv.hpp"

class BlockConfig;

class	Response
{
	public:
		Response();
		Response(Response const &cop);
		virtual ~Response();
  
		Response&	operator=(const Response& cop);

		std::string	getHTTPResponse()
		{
			return (this->_http_response);
		}

		int			getResponseCode() const
		{
			return (this->_response_code);
		}

		void		setResponseCode(int response_code)
		{
			this->_response_code = response_code;
		}

		void	generateResponse(Request &request, ServerConfig const &config);
	private:
		Header					_header;
		std::string				_start_line;
		std::string				_body;
		std::string				_http_response;
		int						_response_code;

		std::string	findPath(Request &request, BlockConfig const &block_config);
		void		write_body(Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path);
		void		write_error_body(BlockConfig const &block_config);
};

#endif