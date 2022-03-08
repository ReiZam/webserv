#include "../webserv.hpp"

/*################################### (Cons-Des)tructor #########################################*/
Uri::Uri() : _scheme(), _userinfo(), _host(), _port(80), _path(), _query(), _fragment() {}

Uri::Uri(std::string& url) : _scheme(), _userinfo(), _host(), _port(80), _path(), _query(), _fragment()
{
	ParseUri(url);
}

Uri::Uri(const Uri& cop) : _scheme(cop._scheme), _userinfo(cop._userinfo), _host(cop._host),
					_port(cop._port), _path(cop._path), _query(cop._query), _fragment(cop._fragment)
{}
Uri::~Uri() {}

Uri&	Uri::operator=(const Uri& cop)
{
    _scheme = cop._scheme;
    _userinfo = cop._userinfo;
    _host = cop._host;
    _port = cop._port;
    _path = cop._path;
	_query = cop._query;
	_fragment = cop._fragment;
	return *this;
}

/*################################### Check #########################################*/
bool		Uri::isvalid_port(std::string ref)
{
	std::string test = ref.substr(0, ref.find("/"));
	for (std::string::const_iterator it = test.begin(); it != test.end(); ++it)
		if (digit.find(*it) == std::string::npos)
			return false;
	return true;
}

/*
	first:  Copy path's uri string split to vector && Check if each path conform
*/
void		Uri::ParsePath(std::string pref)
{
	std::size_t					until;
	
	if (pref == "/")
	{
		_path.push_back(pref);
		pref.clear();
	}
	else
	{
		while (true)
		{
			if ((until = pref.find('/', 1)) != std::string::npos)
			{
				std::string tmp(pref.begin(), pref.begin()+until);
				_path.push_back(pct_decode(tmp));
				pref.erase(0, until);
			}
			else
			{
				_path.push_back(pct_decode(pref));
				pref = pref.substr(until + 1);
				pref.clear();
				break ;
			}
		}
	}
}

/*################################### Utils #########################################*/
std::string		Uri::pct_decode(const std::string& pdir)
{
	std::string	pnew;
	for (std::string::const_iterator it=pdir.begin(); it!=pdir.end(); ++it)
	{
		if (*it == '+')
			pnew += ' ';
		else if (*it == '%')
		{
			++it;
			std::string	hex;
			if (hexdig.find(*it) == std::string::npos || it == pdir.end())
				throw Uri::UriException("Invalid encoded char");
			hex += *it++;
			if (hexdig.find(*it) == std::string::npos || it == pdir.end())
				throw Uri::UriException("Invalid encoded char");
			hex += *it;
			pnew += static_cast<unsigned char>(std::strtol(hex.c_str(), NULL, 16)); //	for replace std::stoi
		}
		else
			pnew += *it;
	}
	return pnew;
}

void		Uri::ParseAuthority(std::string &ref)
{
	std::size_t	spe, ipos;
	if (((spe = ref.find("@")) != std::string::npos))
		ref.erase(0, spe+1);
	if ((ipos = ref.find(":")) == std::string::npos
		&& (ipos = ref.find("/")) == std::string::npos
		&& (ipos = ref.find("?")) == std::string::npos
		&& (ipos = ref.find("#")) == std::string::npos)
					ipos  = ref.size();
	_host = ref.substr(0, ipos);
    std::size_t fport = ref.find(":");
    spe = ref.find("/");
    if (fport != std::string::npos && fport+1 < spe)
    {
		_host.clear();
		_host = ref.substr(0, fport);
		if (!isvalid_port(ref.substr(fport + 1, ref.size())))
			throw(Uri::UriException("Port number invalid"));
		_port = static_cast<u_short>(std::strtol(ref.substr(fport + 1, ref.size()).c_str(), NULL, 10));
	}
	std::transform(_host.begin(), _host.end(), _host.begin(), ::tolower);
	if (spe != std::string::npos)
		ref.erase(0, spe);
}

/*################################### Main Parse #########################################*/
//	assign uri to private variable call in constructor
void		Uri::ParseUri(std::string& ref)
{
	std::size_t	ipos = ref.find("://");
	if (ipos != std::string::npos)
	{
		_scheme = ref.substr(0, ipos);
		std::transform(_scheme.begin(), _scheme.end(), _scheme.begin(), ::tolower);
		ref.erase(0, ipos + 3);
	}
	// IGNORE USERINFO maybe later
	//	https://datatracker.ietf.org:/doc/html/rfc3986#section-3.2
	//	Host && port
	ParseAuthority(ref);
	//	Path
	if ((ipos = ref.find("?")) == std::string::npos
		&& (ipos = ref.find("#")) == std::string::npos)
			ipos  = ref.size();
	std::string tmp = ref.substr(0, ipos);
	if (tmp.size() == 0)
		_path.push_back("/");
	else
		ParsePath(tmp);
	ref.erase(0, ipos);
	//	Query
	if ((ipos = ref.find("?")) != std::string::npos)
	{
		while (true)
		{
			if ((ipos = ref.find("=")) != std::string::npos)
			{
				std::string	key = pct_decode(ref.substr(1, ipos - 1));
				ref.erase(0, ipos + 1);
				if ((ipos = ref.find("&")) == std::string::npos
					&& (ipos = ref.find("#")) == std::string::npos)
						ipos = ref.size();
				std::string	value = pct_decode(ref.substr(0, ipos));
				_query[key]=value;
				ref.erase(0, ipos);
			}
			else
				break ;
		}
	}
	//	FRAGMENT
	if ((ipos = ref.find("#")) != std::string::npos)
	{
		_fragment = pct_decode(ref.substr(1, ref.size()));
		ref.clear();
	}
}

std::string		Uri::AllPath(void) const
{
	if (_path.empty())
		return ("/");
	std::string	res;
	for (std::vector<std::string>::const_iterator it=_path.begin(); it!=_path.end(); ++it)
		res += *it;
	return res;
}

std::string		Uri::AllQuery(void) const
{
	std::string res;

	for (std::map<std::string, std::string>::const_iterator it = _query.begin();it != _query.end();++it)
	{
		if (it != _query.begin())
			res += "&";
		res += (*it).first + "=" + (*it).second;
	}
	// std::cout << res << std::endl;
	return res;
}

std::string		Uri::AllUri(void) const
{
	std::string allUri = this->AllPath();
	std::string allQuery = this->AllQuery();

	if (!allQuery.empty())
		allUri += "?" + allQuery;
	return (allUri);
}
