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
  
		Response&						operator=(const Response& cop);

		std::string						getRawHeader()
		{
			return (this->_raw_header);
		}

		int								getResponseCode() const
		{
			return (this->_response_code);
		}

		bool							isValidResponseCode() const
		{
			return (this->_response_code >= 200 && this->_response_code < 300);
		}

		void							setResponseCode(int response_code)
		{
			this->_response_code = response_code;
		}

		std::vector<unsigned char> &	getBody()
		{
			return (this->_body);
		}
		
		void		generateResponse(Request &request, ServerConfig const &config);
	private:
		Header						_header;
		std::string					_raw_header;
		std::vector<unsigned char>	_body;
		int							_response_code;

		std::string	findPath(Request &request, ServerConfig const &config, BlockConfig const &block_config);
		void		write_body(Request &request, ServerConfig const &config, BlockConfig const &block_config, std::string path);
		void		write_error_body(ServerConfig const &config, BlockConfig const &block_config);
};

#endif