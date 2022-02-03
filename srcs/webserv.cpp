#include "webserv.hpp"

void	signal_handler(int sign)
{
	(void)sign;
    std::cout << "[Webserv] Exiting..." << std::endl;

    exit(EXIT_FAILURE);
}

int	getGlobalMaxFD(std::vector<Server> &servers)
{
    int max_fd = 0;

	for (std::vector<Server>::iterator it = servers.begin();it != servers.end();it++)
		if ((*it).getMaxFD() > max_fd)
			max_fd = (*it).getSocketFD();
    return (max_fd);
}

void	init_webserv(std::vector<Server> &servers, std::map<std::string, ServerConfig> servers_config, fd_set *rset)
{
	for (std::map<std::string, ServerConfig>::iterator it = servers_config.begin();it != servers_config.end();it++)
	{
		Server new_server((*it).second);

		new_server.init();
		servers.push_back(new_server);
		FD_SET(new_server.getSocketFD(), rset);
	}
}

void	run_webserv(std::vector<Server> &servers, fd_set *rset, fd_set *wset)
{
	struct	timeval _time;

	_time.tv_sec = 1;
	_time.tv_usec = 0;
	while (1)
	{
		if (select(getGlobalMaxFD(servers), rset, wset, NULL, &_time) < 0)
			throw WebservException("select()", std::string(strerror(errno)));
		for (std::vector<Server>::iterator it = servers.begin();it != servers.end();it++)
			if (FD_ISSET((*it).getSocketFD(), rset))
				(*it).run(wset, rset);
	}
}


int main(int ac, char **av)
{
	fd_set rset, wset;
	
	if (ac == 2)
	{
		Config config(av[1]);

		try
		{
			config.init();
			config.parse();
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}

		signal(SIGINT, &signal_handler);

		FD_ZERO(&rset);
		FD_ZERO(&wset);

		std::vector<Server>	servers;

		try
		{
			init_webserv(servers, config.getServersConfig(), &rset);
			run_webserv(servers, &rset, &wset);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
		return (EXIT_SUCCESS);
	}
	else
		std::cout << "[Webserv] Usage: ./webserv <config_file>" << std::endl;
	return (EXIT_FAILURE);
}