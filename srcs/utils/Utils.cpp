#include "../webserv.hpp"

std::string	read_fd(int fd)
{
	std::string result;
	int ret;
	char buffer[BUFFER_SIZE + 1];

	ret = 0;
	while ((ret = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[ret] = 0;
		result = result + buffer;
	}
	return (result);
}

long	get_current_time()
{
	struct timeval time_c;

    gettimeofday(&time_c, 0);
    return (time_c.tv_sec);
}

std::vector<std::string> split_vect(std::string const &string, char delimiteur)
{
	std::vector<std::string> str;
    size_t begin, end;

    end = 0;
    
    while ((begin = string.find_first_not_of(delimiteur, end)) != std::string::npos){
        end = string.find(delimiteur, begin);
    str.push_back(string.substr(begin, end - begin));      
    }
    return str;	
}

std::string trim(std::string const &str)
{
	size_t f, l;
    f = str.find_last_of('.');
    l = str.find_last_of(' ');
    return str.substr(f + 1, (l - f + 1));
	
}