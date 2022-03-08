#include "../webserv.hpp"

void	ClientHandler::handleRequest(Client &client, Server &server)
{
	if (client.getRequest().GetStep() == START)
	{
		std::string request_header = client.getRequestHeader();

		if (!request_header.empty())
		{
			try
			{
				client.getRequest().ParseHeader(request_header);
				client.getRequest().chooseServerConfig(server.getConfigs());
			}
			catch(const std::exception& e)
			{
				client.getRequest().setErrorCode(BAD_REQUEST);
			}
		}
		else
			client.getRequest().setErrorCode(BAD_REQUEST);
	}

	if (!client.getRequest().isFinished() && client.getRequest().GetStep() == BODY)
	{
		if (client.getRequest().GetHeader().IsValueSetTo("Transfer-Encoding", "chunked"))
			client.getRequest().ParseChunked(client.getRequestBody());
		else if ((client.getRequest().GetMethod() == "POST" && client.getRequest().ValidPost(client.getStringRequest()))
				|| (client.getRequest().GetMethod() == "GET"))
		{
			std::vector<unsigned char> &body = client.getRequest().GetBody();

			for (std::vector<unsigned char>::iterator it = client.getBodyBegin();it != client.getBinaryRequest().end();it++)
				body.push_back(*it);

			client.getRequest().ValidBody();
		}
	}
}

void	ClientHandler::handleResponse(Client &client, Server &server)
{
	(void)server;
	client.getResponse().setResponseCode(client.getRequest().GetErrorCode());
	client.getResponse().generateResponse(client, client.getRequest(), client.getRequest().GetCurrentConfig());

	if (client.getResponse().isValidResponseCode())
		client.resetErrorCounter();
	else
		client.incrementErrorCounter();
}
