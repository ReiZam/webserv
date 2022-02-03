#include "webserv.hpp"

int main(int ac, char **av)
{
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
			std::cout << e.what() << std::endl;
		}
	}
}