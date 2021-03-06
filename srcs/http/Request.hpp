#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <webserv.hpp>

#define START 0
#define HEADER 1
#define BODY 2
#define END 3

class ServerConfig;

//	Request
class	Request
{
	private:
		int						_step;
		int						_scode;

		std::string					_method;
		std::string					_version;
		std::string					_raw_path;
		std::string					_start_line;
		std::string					_boundary;
        Header						_header;
		std::vector<unsigned char>	_body;
		Uri							_uri;
		bool						_ishost;
		ServerConfig 				_current_config;
	public:
		//	Cons-Destructor
		Request();
		Request(const Request& cop);
		virtual ~Request();
		//	Assign
		Request&	operator=(const Request& cop);

		//	Getter
		std::string	GetStartLine(void) const	{ return _start_line; }
		std::string	GetMethod(void)	{ return _method; }
		std::string	GetVersion(void) { return _version; }
		std::string	GetRawPath(void) { return _raw_path; }
		std::string	GetBoundary(void) { return _boundary; }
		int			GetErrorCode(void) { return _scode; }
		Header		GetHeader(void)	{ return _header; }
		Uri			GetUri(void)	{ return _uri; }
		int			GetStep(void)	{ return _step; }
		bool		GetIsHost(void) const	{ return _ishost; }
		std::vector<unsigned char> &	GetBody(void) { return _body; }
		ServerConfig &					GetCurrentConfig(void) { return _current_config; }

		// Setter
		void		setErrorCode(int code) { _scode = code; }
		void		setStep(int step) { _step = step; }
		void		setBody(std::vector<unsigned char> body) { _body = body; }

		//	Valid Parse
		bool		isValidHeader(void) const;
		void		ValidBody();
		bool		ValidPost(std::string string_request);
		bool		ParseChunked(std::string request_body);
		bool		BadChunked(const std::string next, const size_t count) const;
		bool		CheckHeader();
		void		ParseStartLine();
		bool		CheckStartLine();
		bool		CheckGlobalHeader(std::string http_header);
		void		chooseServerConfig(std::vector<ServerConfig> const &configs);

        //  Function
        void    ParseHeader(std::string http_header);

		//	Utils
		bool	isFinished() { return (this->_scode != OK || this->_step == END); }

		//	Exception
		class RequestExcept : public std::exception
		{
			private:
				const char	*_err;
			public:
				explicit RequestExcept(const char *excep) : _err(excep) {}
				virtual ~RequestExcept() throw() {}
				virtual const char*	what() const throw() { return _err; }
		};
};

#endif
