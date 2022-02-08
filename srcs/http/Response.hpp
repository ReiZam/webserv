# ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../webserv.hpp"

class	Response
{
	public :

		std::string		        _start_line;	//	status-code && reason-phrase
		int						_response_code;
		std::string _cgi_body;
		std::map<std::string, std::string> _cgi_response_header;
		std::map<std::string, std::string> _response_header;	
		//	Cons-Destructor
		Response();
		// Response(Request& req);		//	maybe
		Response(Response const &cop);
		Response &operator=(Response const &cop);
		std::string	gen_status_code(int code);
		virtual ~Response();

		
		//	Assign
		void		clear_response(void);

		enum code {
		//	1xx - Information
		CONTINUE = 100,
		SWITCHING_PROTOCOLS = 101,
		//	2xx Success
		OK = 200,								//	https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/200
		CREATED = 201,
		ACCEPTED = 202,
		NON_AUTHORITATIVE_INFORMATION = 203,
		NO_CONTENT = 204,
		RESET_CONTENT = 205,
		PARTIAL_CONTENT = 206,
		//	3xx Redirection
		MULTIPLE_CHOICES = 300,
		MOVED_PERMANENTLY = 301,
		FOUND = 302,
		SEE_OTHER = 303,
		NOT_MODIFIED = 304,
		USE_PROXY = 305,
		SWITCH_PROXY = 306,
		TEMPORARY_REDIRECT = 307,
		PERMANENT_REDIRECT = 308,
		//	4xx Client Error
		BAD_REQUEST = 400,
		UNAUTHORIZED = 401,
		PAYMENT_REQUIRED = 402,
		FORBIDDEN = 403,
		NOT_FOUND = 404,
		METHOD_NOT_ALLOWED = 405,
		NOT_ACCEPTABLE = 406,
		PROXY_AUTHENTICATION_REQUIRED = 407,
		REQUEST_TIMEOUT = 408,
		CONFLICT = 409,
		GONE = 410,
		LENGTH_REQUIRED = 411,
		PRECONDITION_FAILED = 412,
		REQUEST_ENTITY_TOO_LARGE = 413,			//	client max body size
		REQUEST_URI_TOO_LONG = 414,
		UNSUPPORTED_MEDIA_TYPE = 415,
		REQUESTED_RANGE_NOT_SATISFIABLE = 416,
		EXPECTATION_FAILED = 417,
		//	Server Error 5xx
		INTERNAL_SERVER_ERROR = 500,
		NOT_IMPLEMENTED = 501,
		BAD_GATEWAY = 502,
		SERVICE_UNAVAILABLE = 503,
		GATEWAY_TIMEOUT = 504,
		HTTP_VERSION_NOT_SUPPORTED = 505
		};
};

#endif
