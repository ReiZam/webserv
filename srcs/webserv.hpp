#ifndef WEBSERV_HPP
# define WEBSERV_HPP

enum code
{
	CONTINUE = 100,
	SWITCHING_PROTOCOLS = 101,
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NON_AUTHORITATIVE_INFORMATION = 203,
	NO_CONTENT = 204,
	RESET_CONTENT = 205,
	PARTIAL_CONTENT = 206,
	MULTIPLE_CHOICES = 300,
	MOVED_PERMANENTLY = 301,
	FOUND = 302,
	SEE_OTHER = 303,
	NOT_MODIFIED = 304,
	USE_PROXY = 305,
	SWITCH_PROXY = 306,
	TEMPORARY_REDIRECT = 307,
	PERMANENT_REDIRECT = 308,
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
	REQUEST_ENTITY_TOO_LARGE = 413,
	REQUEST_URI_TOO_LONG = 414,
	UNSUPPORTED_MEDIA_TYPE = 415,
	REQUESTED_RANGE_NOT_SATISFIABLE = 416,
	EXPECTATION_FAILED = 417,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAILABLE = 503,
	GATEWAY_TIMEOUT = 504,
	HTTP_VERSION_NOT_SUPPORTED = 505
};

#include <sys/wait.h> 
#include <sys/time.h>
#include <poll.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <cctype>
#include <sys/stat.h>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <list>
#include <deque>
#include <cstddef>
#include <stack>
#include <stddef.h>
#include <map>  
#include <fstream>
#include <errno.h>
#include <string.h>
#include <utility>
#include <algorithm>
#include <iterator>
#include <sys/socket.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>

// UTILS
#include "utils/Utils.hpp"

// HTTP
# include "http/Uri.hpp"
# include "http/Header.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"

// CONFIG
# include "config/BlockConfig.hpp"
# include "config/LocationConfig.hpp"
# include "config/ServerConfig.hpp"
# include "config/ConfigLexer.hpp"
# include "config/Config.hpp"

// SERVER
# include "server/ClientHandler.hpp"
# include "server/Client.hpp"
# include "server/Server.hpp"

# define BUFFER_SIZE 4096
# define WEBSERV_VERSION "webserv/1.0.0"

const std::string LOCALHOST("0.0.0.0");

# define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

std::string					read_fd(int fd);
long						get_current_time();
bool						check_http_req_end(std::string request);
bool						ends_with(std::string const &value, std::string const &ending);
bool						starts_with(std::string const &value, std::string const &starts);
bool						exist_file(std::string const &path);
bool						exist_directory(std::string const &path);
char*						get_file_content(std::string const &path);
std::vector<unsigned char>	string_to_uchar_vec(std::string str);
bool						check_ext_path(std::string file_path, std::string extension);
std::string					gen_status_code(int	code);
std::vector<unsigned char>	gen_html_error_page(int code);
std::string					GetDate(void);
std::string     			GetLastModifiedDate(const std::string& fpath);
std::string					rdfile(const char* file);
std::vector<unsigned char> 	read_file(const char* filename);
std::string					get_path_ext(std::string path);
Header						parse_header(std::string header_string);
bool						check_forbidden_characters_string(std::string string, std::string forbidden_characters);

class WebservException : public std::exception
{
	public:
		WebservException(std::string error_category, std::string error_text)
		{
			this->error = "[WebservException] " + error_category +  ": " + error_text;
        }

        virtual ~WebservException() throw() {};

        virtual const char *what() const throw(){
            return (this->error.c_str());
        };
    private:
    	std::string error;
};

extern std::vector<Server*> servers;

#endif
