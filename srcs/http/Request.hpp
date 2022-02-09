#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Header.hpp"
#include "Uri.hpp"

#define START 0
#define HEADER 1
#define BODY 2
#define END 3

//	Request
class	Request
{
	private:
		int						_step;			//	check step if the header has not complete
		int						_scode;			//	error code  that server should return about the request

		std::string				_start_line;	//	method && request
        Header					_header;
        std::string				_body;
		Uri						_uri;
		bool					_ishost;		//	just to check is present if uri's not complete
        // Cgi					_cgi;       //  to do
	public:
		//	Cons-Destructor
		Request();
		Request(const Request& cop);
		virtual ~Request();
		//	Assign
		Request&	operator=(const Request& cop);

		//	Getter
		std::string	GetStartLine(void) const	{ return _start_line; }
		std::string	GetMethod(void)	{ return _start_line.substr(0, _start_line.find(" ")); }
		std::string	GetVersion(void) { return _start_line.substr(_start_line.find(" H") + 1); }
		Header		GetHeader(void)	{ return _header; }
		Uri			GetUri(void)	{ return _uri; }
		std::string	GetBody(void)	{ return _body; }
		bool		GetIsHost(void) const	{ return _ishost; }

		//	Valid Parse
		void		ValidStartLine(std::string ref);
		bool		isValidHeader(void) const;		//	to do

        //  Function
		void	ParseBody(std::string body);
        void    ParseRequest(std::string href);		// in progress

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
