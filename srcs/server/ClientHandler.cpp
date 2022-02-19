#include "../webserv.hpp"

void	ClientHandler::handleRequest(std::string http_request, Client &client, Server const &server)
{
	if (check_http_req_end(http_request))
		client.getRequest().ParseRequest(http_request, server.getConfig());
	else
		client.getRequest().setErrorCode(400);
	(void)server;
}

void	ClientHandler::handleResponse(Client &client, Server const &server)
{
	client.getResponse().setResponseCode(client.getRequest().GetErrorCode());
	client.getResponse().generateResponse(client.getRequest(), server.getConfig());
}