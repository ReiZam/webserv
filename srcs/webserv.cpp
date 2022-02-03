#include "config/Config.hpp"

int main(int ac, char **av)
{
	Config config(av[1]);

	try
	{
		config.init();
		config.parse();
	} catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}


}