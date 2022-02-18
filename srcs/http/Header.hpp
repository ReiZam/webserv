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

	bool					IsValueSetTo(const std::string &key, std::string const &compared_value);
    //	To Set Display Header
    std::string		HtoStr(void) const;
};

#endif
