#include "../webserv.hpp"


Header::Header() : _hline()
{
}

Header::Header(const Header& cop)
{
	_hline.insert(cop._hline.begin(), cop._hline.end());
}

Header&		Header::operator=(const Header& cop)
{
	_hline = cop._hline;
	return *this;
}

Header::~Header()
{}


//  Utils
std::string		Header::HtoStr(void) const
{
    std::string res;
    for (std::map<std::string,std::string>::const_reverse_iterator it=_hline.rbegin(); it!=_hline.rend(); ++it)
        res += it->first + ": " + it->second + crlt;
    // res += crlt;
    return res;
}

