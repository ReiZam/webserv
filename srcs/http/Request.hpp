#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../webserv.hpp"

//	Request
class	Request
{
	private:
		std::string				_start_line;	//	method && request
        Header					_header;
        std::string				_body;
		Uri						_uri;
		bool					_ishost;
        // Cgi					_cgi;       //  to do
	public:
		//	Cons-Destructor
		Request();
		Request(const Request& cop);
		virtual ~Request();
		//	Assign
		Request&	operator=(const Request& cop);

        //  Function
        void    ParseRequest(std::string href);		// to do

		//	Utils
		bool	ValidStartLine(void);

		//	Exception
		class RequestExcept : public std::exception
		{
			private:
				const char	*_err;
			public:
				RequestExcept(const char *excep) : _err(excep) {}
				virtual ~RequestExcept() throw() {}
				virtual const char*	what() const throw() { return _err; }
		};
};

#endif
