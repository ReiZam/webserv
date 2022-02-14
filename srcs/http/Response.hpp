#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../webserv.hpp"

class	Response
{
	private:
	public :

		std::string		        			_start_line;	//	status-code && reason-phrase
		int									_response_code;
		std::string 						_cgi_body;
		std::string 						_body;
		std::map<std::string, std::string> 	_cgi_response_header;
		std::map<std::string, std::string> 	_response_header;	
		//	Cons-Destructor
		Response();
		// Response(Request& req);		//	maybe
		Response(Response const &cop);
		virtual ~Response();
  
		Response&	operator=(const Response& cop);

		int		getResponseCode() const
		{
			return (this->_response_code);
		}

		void	setResponseCode(int response_code) { this->_response_code = response_code; }

	private:
		// std::string		        _start_line;	//	status-code && reason-phrase
		// int						_response_code;

		void		clear_response(void);
  
};

#endif
