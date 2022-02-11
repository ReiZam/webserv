#ifndef HEADER_HPP
#define HEADER_HPP

#include "../webserv.hpp"

//	https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods

//	https://raw.githubusercontent.com/Working-From-Home/webserv/uml/docs/diagrams/out/webserv.svg
//	https://en.wikipedia.org/wiki/Uniform_Resource_Identifier
/*	https://datatracker.ietf.org/doc/html/rfc733#section-2 
	https://datatracker.ietf.org/doc/html/rfc3986#appendix-A
	https://datatracker.ietf.org/doc/html/rfc5234#appendix-B.1
*/
const std::string	alpha("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const std::string	digit("0123456789");
const std::string	hexdig("0123456789abcdefABCDEF");
const std::string	crlt("\r\n");
const std::string	wsp(" \t");
const std::string	gen_delims(":/?#[]@");		//	https://datatracker.ietf.org/doc/html/rfc3986#section-2.2
const std::string	sub_delims("!$&'()*+,;=");	//	^	^	^	^	^	^	^	^	^	^	^	^	^	^	^

const std::string	reserved(gen_delims + sub_delims);
const std::string	unreserved(alpha + digit + "-._~");
const std::string	pchar(unreserved + sub_delims + ":@");	//	miss pct-encodedc= "%+1hex+1hex"
const std::string	query_frament(pchar + "/?");

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
	REQUEST_ENTITY_TOO_LARGE = 413,			//	client max body size = 1 000 000->default
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

class	Header
{
private:
	std::map<std::string, std::string>	_hline;
public:
	//	Canonical Form
	Header();
	Header(std::string& head);
	Header(const Header& cop);
	virtual ~Header();
	Header&		operator=(const Header& cop);
	//	Setter & Getter
	void					SetValue(const std::string& hname, const std::string& hvalue) { _hline[hname] = hvalue; }
	std::map<std::string,std::string>	GetHeader(void) const	{ return _hline; }
	const std::string		GetValue(const std::string& key)	{ return _hline[key]; }
	//	To Set Display Header
	std::string		HtoStr(void) const;
};

/*
	Utils
*/
//	Get current time GMT
std::string		GetDate(void);
//	Get last time in GMT directory/files has modified
//	@param-> AllPath in Uri or file to check
std::string     GetLastModifiedDate(const std::string& fpath);

#endif
