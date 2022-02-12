#ifndef WEBSERV_HPP
# define WEBSERV_HPP


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

// CONFIG
# include "config/BlockConfig.hpp"
# include "config/LocationConfig.hpp"
# include "http/Header.hpp"
# include "http/Uri.hpp"
# include "http/Request.hpp"
# include "config/ServerConfig.hpp"
# include "config/ConfigLexer.hpp"
# include "config/Config.hpp"

// HTTP
# include "http/Uri.hpp"
# include "http/Header.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"

// SERVER
# include "client/Client.hpp"
# include "http/CGI.hpp"
# include "server/Server.hpp"

# define BUFFER_SIZE 4096

std::string			read_fd(int fd);
long				get_current_time();
bool				check_http_req_end(std::string request);

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

#endif