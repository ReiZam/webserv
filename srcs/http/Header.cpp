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


//  General Utils
std::string     GetDate(void)
{
	char		buf[256];
	time_t		rtime = time(&rtime);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", gmtime(&rtime));
	return std::string(buf);
}

//	https://stackoverflow.com/questions/21159047/get-the-creation-date-of-file-or-folder-in-c
std::string     GetLastModifiedDate(const std::string& fpath)
{
	struct stat	result;
	char		buf[256];
	if(stat(fpath.c_str(), &result) == 0)
	{
		strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", gmtime(&result.st_ctime));
		return std::string(buf);
	}
	return std::string("");
}
