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