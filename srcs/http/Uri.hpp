#ifndef URI_HPP
#define URI_HPP

#include "Header.hpp"

class Uri
{
	private:
		std::string							_scheme;
		std::string							_userinfo;
		std::string							_host;
		u_short								_port;
		std::vector<std::string>			_path;
		std::map<std::string, std::string>	_query;
		std::string							_fragment;
	public:
		//	Constructor
		Uri();
        Uri(std::string& url);
        Uri(const Uri& cop);
		~Uri();

		Uri&	operator=(const Uri& cop);

		//	Getter
		std::string			GetScheme() const	{ return _scheme; }
		std::string			GetUserInfo() const	{ return _userinfo; }
		std::string			GetHost() const		{ return _host; }
		u_short				GetPort() const		{ return _port; }
		std::vector<std::string>			GetPath() const	 { return _path; }
		std::map<std::string, std::string>	GetQuery() const { return _query; }
		std::string			GetFragment() const	{ return _fragment; }
		
		//	Exception
		class UriException : public std::exception
		{
			private:
				const char	*_err;
			public:
				UriException(const char *excep) : _err(excep) {}
				virtual ~UriException() throw() {}
				virtual const char*	what() const throw() { return _err; }
		};
		bool		isvalid_port(std::string ref);
		void		ParseUri(std::string& ref);						//	maybe modif ret
		void		ParsePath(std::string pref);
		std::string	pct_decode(const std::string& pdir);

		std::string		AllPath(std::vector<std::string> rpath);	//	To do
		// void		ParseAuthority(std::string& ref);
};

#endif