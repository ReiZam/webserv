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

# include "config/LocationConfig.hpp";
# include "config/ServerConfig.hpp";
# include "config/ConfigLexer.hpp";
# include "config/Config.hpp"

#endif