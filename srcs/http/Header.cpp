#include "Header.hpp"

HttpHeader::HttpHeader() : _hline()
{
}

HttpHeader::HttpHeader(const HttpHeader& cop)
{
	_hline.insert(cop._hline.begin(), cop._hline.end());
}

HttpHeader&		HttpHeader::operator=(const HttpHeader& cop)
{
	_hline = cop._hline;
	return *this;
}

HttpHeader::~HttpHeader()
{}

std::string		HttpHeader::HtoStr(void) const
{
    std::string res;
    for (std::map<std::string,std::string>::const_iterator it=_hline.begin(); it!=_hline.end(); ++it)
        res += it->first + ": " + it->second + crlt;
    res += crlt;
    return res;
}
