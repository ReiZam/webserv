#ifndef CLIENT_HANDLER_HPP
# define CLIENT_HANDLER_HPP

class Server;
class Client;

class ClientHandler
{
	public:
		ClientHandler() {}

		void	handleRequest(Client &client, Server &server);
		void	handleResponse(Client &client, Server &server);
};

#endif
