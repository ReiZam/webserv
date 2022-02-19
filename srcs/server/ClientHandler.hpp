#ifndef CLIENT_HANDLER_HPP
# define CLIENT_HANDLER_HPP

class Server;
class Client;

class ClientHandler
{
	public:
		ClientHandler() {}

		void	handleRequest(std::string http_request, Client &client, Server const &server);
		void	handleResponse(Client &client, Server const &server);
};

#endif