#include "webserv.hpp"

std::vector<Server*>	servers;
Config					*global_config;

void	signal_handler(int sign)
{
	(void)sign;
	for (std::vector<Server*>::iterator it = servers.begin();it != servers.end();)
		delete (*(it++));
	delete global_config;
	std::cout << "[Webserv] Exiting..." << std::endl;
    exit(EXIT_FAILURE);
}

int	getGlobalMaxFD()
{
    int max_fd = 0;

	for (std::vector<Server*>::iterator it = servers.begin();it != servers.end();it++)
		if ((*it)->getMaxFD() > max_fd)
			max_fd = (*it)->getMaxFD();
    return (max_fd + 1);
}

void	init_webserv(fd_set *rset)
{
	std::vector<ServerConfig> servers_config = global_config->getServersConfig();

	for (std::vector<ServerConfig>::iterator it = servers_config.begin();it != servers_config.end();it++)
	{
		Server *new_server = new Server((*it));

		new_server->init();
		servers.push_back(new_server);
		FD_SET(new_server->getSocketFD(), rset);
	}
}

void	run_webserv(fd_set *rset, fd_set *wset)
{
	struct	timeval _time;

	_time.tv_sec = 1;
	_time.tv_usec = 0;
	while (1)
	{
		try
		{
			if (select(getGlobalMaxFD(), rset, wset, NULL, &_time) < 0)
				throw WebservException("select()", std::string(strerror(errno)));
			for (std::vector<Server*>::iterator it = servers.begin();it != servers.end();it++)
				(*it)->run(rset, wset);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

int main(int ac, char **av)
{
	fd_set rset, wset;

	if (ac == 2)
	{
		global_config = new Config(av[1]);

		try
		{
			global_config->init();
			global_config->parse();
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			delete global_config;
			return (EXIT_FAILURE);
		}

		signal(SIGINT, &signal_handler);
		signal(SIGPIPE, SIG_IGN);

		FD_ZERO(&rset);
		FD_ZERO(&wset);

		try
		{
			init_webserv(&rset);
			run_webserv(&rset, &wset);
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
