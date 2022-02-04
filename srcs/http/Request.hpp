#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Header.hpp"
#include "Uri.hpp"

//	Request
class	Request
{
	private:
		std::string				_start_line;	//	method && request
        HttpHeader              _header;
        std::string             _body;
		Uri						_uri;
        // Cgi                      _cgi;       //  to do
	public:
		//	Cons-Destructor
		Request();
		Request(const Request& cop);
		virtual ~Request();
		//	Assign
		Request&	operator=(const Request& cop);

        //  Function
        void    ParseRequest(std::string href);		// to do

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
