#include "../webserv.hpp"

void	ClientHandler::handleRequest(std::string http_request, Client &client, Server const &server)
{
	(void)http_request;
	(void)client;
	(void)server;
}

void	ClientHandler::handleResponse(Client &client, Server const &server)
{
	(void)client;
	(void)server;
}