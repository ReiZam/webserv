#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../webserv.hpp"

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

std::string		gen_status_code(int	code) {
	switch (code) {
		//	1xx - Information -> https://datatracker.ietf.org/doc/html/rfc7231#section-6.2
		case CONTINUE: 
			return "100 Continue";
		case SWITCHING_PROTOCOLS:
			return "101 Switching Protocols";
		//	2xx Success -> https://datatracker.ietf.org/doc/html/rfc7231#section-6.3
		case OK:
			return "200 OK";
		case CREATED:
			return "201 Created";
		case ACCEPTED:
			return "202 Accepted";
		case NON_AUTHORITATIVE_INFORMATION:
			return "203 Non-Authoritative Information";
		case NO_CONTENT:
			return "204 No Content";
		case RESET_CONTENT:
			return "205 Reset Content";
		case PARTIAL_CONTENT:
			return "206 Partial Content";
		//	3xx Redirection -> https://datatracker.ietf.org/doc/html/rfc7231#section-6.4
		case MOVED_PERMANENTLY:
			return "301 Moved Permanently";
		case FOUND:
			return "302 Found";
		case SEE_OTHER:
			return "303 See Other";
		case NOT_MODIFIED:
			return "304 Not Modified";
		case USE_PROXY:
			return "305 Use Proxy";
		case SWITCH_PROXY:
			return "306 Switch Proxy";
		case TEMPORARY_REDIRECT:
			return "307 Temporary Redirect";
		//	4xx Client Error -> https://datatracker.ietf.org/doc/html/rfc7231#section-6.5
		case BAD_REQUEST:
			return "400 Bad Request";
		case UNAUTHORIZED:
			return "401 Unauthorized";
		case PAYMENT_REQUIRED:
			return "402 Payment Required";
		case FORBIDDEN:
			return "403 Forbidden";
		case NOT_FOUND:
			return "404 Not Found";
		case METHOD_NOT_ALLOWED:
			return "405 Method Not Allowed";
		case NOT_ACCEPTABLE:
			return "406 Not Acceptable";
		case PROXY_AUTHENTICATION_REQUIRED:
			return "407 Proxy Authentication Required";
		case REQUEST_TIMEOUT:
			return "408 Request Timeout";
		case CONFLICT:
			return "409 Conflict";
		case GONE:
			return "410 Gone";
		case LENGTH_REQUIRED:
			return "411 Length Required";
		case PRECONDITION_FAILED:
			return "412 Precondition Failed";
		case REQUEST_ENTITY_TOO_LARGE:
			return "413 Request Entity Too Large";
		case REQUEST_URI_TOO_LONG:
			return "414 Request-URI Too Long";
		case UNSUPPORTED_MEDIA_TYPE:
			return "415 Unsupported Media Type";
		case REQUESTED_RANGE_NOT_SATISFIABLE:
			return "416 Requested Range Not Satisfiable";
		case EXPECTATION_FAILED:
			return "417 Expectation Failed";
		//	Server Error 5xx -> https://datatracker.ietf.org/doc/html/rfc7231#section-6.6
		case INTERNAL_SERVER_ERROR:
			return "500 Internal Server Error";
		case NOT_IMPLEMENTED:
			return "501 Not Implemented";
		case BAD_GATEWAY:
			return "502 Bad Gateway";
		case SERVICE_UNAVAILABLE:
			return "503 Service Unavailable";
		case GATEWAY_TIMEOUT:
			return "504 Gateway Timeout";
		case HTTP_VERSION_NOT_SUPPORTED:
			return "505 HTTP Version Not Supported";
		default:
			return "";
	}
}

//	Response
class	Response
{
	public:
		//	Cons-Destructor
		Response();
		// Response(Request& req);		//	maybe
		Response(const Response& cop);
		virtual ~Response();
		//	Assign
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
		std::string		        _start_line;	//	status-code && reason-phrase
		int						_response_code;
};

#endif
