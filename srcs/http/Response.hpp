#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../webserv.hpp"

class	Response
{
	public:
		Response();
		Response(Response const &cop);
		virtual ~Response();
  
		Response&	operator=(const Response& cop);

		int		getResponseCode() const
		{
			return (this->_response_code);
		}

		void	setResponseCode(int response_code)
		{
			this->_response_code = response_code;
		}
	private:
		std::string		        _start_line;
		int						_response_code;
};

#endif