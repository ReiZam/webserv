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

std::string		Header::HtoStr(void) const
{
    std::string res;
    for (std::map<std::string,std::string>::const_iterator it=_hline.begin(); it!=_hline.end(); ++it)
        res += it->first + ": " + it->second + crlt;
    res += crlt;
    return res;
}
