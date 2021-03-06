#include <webserv.hpp>

std::vector<Server*>	servers;
Config					*global_config;

void	free_webserv_data(void)
{
	for (std::vector<Server*>::iterator it = servers.begin();it != servers.end();)
		delete (*(it++));
	delete global_config;
	servers.clear();
}

void	signal_handler(int sign)
{
	(void)sign;
	free_webserv_data();
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
	(void)rset;
	std::vector<ServerConfig> servers_config = global_config->getServersConfig();
	std::map<int, bool> server_created;

	for (std::vector<ServerConfig>::iterator it = servers_config.begin();it != servers_config.end();it++)
	{
		if (server_created.find((*it).getPort())->second == true) continue ;

		std::vector<ServerConfig> configs;

		for (std::vector<ServerConfig>::iterator cfg_it = servers_config.begin();cfg_it != servers_config.end();cfg_it++)
			if ((*cfg_it).getPort() == (*it).getPort())
				configs.push_back(*cfg_it);
		Server *new_server = new Server(configs, (*it).getPort(), (*it).getHost(), (*it).getAddress());
		servers.push_back(new_server);

		new_server->init();
		FD_SET(new_server->getSocketFD(), rset);
		server_created[(*it).getPort()] = true;
	}
}

void	run_webserv(fd_set *rset, fd_set *wset)
{
	struct	timeval _time;

	_time.tv_sec = 5;
	_time.tv_usec = 0;
	while (servers.size())
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
			free_webserv_data();
			return (EXIT_FAILURE);
		}
		return (EXIT_SUCCESS);
	}
	else
		std::cout << "[Webserv] Usage: ./webserv <config_file>" << std::endl;
	return (EXIT_FAILURE);
}
