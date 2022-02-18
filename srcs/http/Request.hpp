#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../webserv.hpp"

#define START 0
#define HEADER 1
#define BODY 2
#define END 3

class ServerConfig;

//	Request
class	Request
{
	private:
		int						_step;			//	check step if the header has not complete
		int						_scode;			//	error code  that server should return about the request

		std::string				_method;
		std::string				_version;
		std::string				_raw_path;
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
		std::string	GetMethod(void)	{ return _method; }
		std::string	GetVersion(void) { return _version; }
		std::string	GetRawPath(void) { return _raw_path; }
		int			GetErrorCode(void) { return _scode; }
		Header		GetHeader(void)	{ return _header; }
		Uri			GetUri(void)	{ return _uri; }
		std::string	GetBody(void)	{ return _body; }
		bool		GetIsHost(void) const	{ return _ishost; }

		// Setter
		void		setErrorCode(int code) { this->_scode = code; }

		//	Valid Parse
		void		ValidStartLine(std::string ref);
		bool		isValidHeader(void) const;		//	to do

        //  Function
		void	ParseBody(std::string body, ServerConfig const &config);
        void    ParseRequest(std::string href, ServerConfig const &config);		// in progress

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
