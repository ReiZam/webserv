#include "../webserv.hpp"

void	ClientHandler::handleRequest(std::string http_request, Client &client, Server const &server)
{
	if (check_http_req_end(http_request))
	{
		client.getRequest().ParseRequest(http_request);
		client.getResponse().setResponseCode(client.getRequest().GetErrorCode());
	}
	else
		client.getResponse().setResponseCode(400);
	(void)server;
}

void	ClientHandler::handleResponse(Client &client, Server const &server)
{
	const char s[] = "HTTP/1.1 200 OK\nServer: webserv\nDate: Fri, 04 Feb 2022 03:45:11 GMT\nContent-Type: text/html; charset=utf-8\nContent-Length: 24\nLast-Modified: Tue, 25 Jan 2022 15:41:20 GMT\nConnection: keep-alive\n\n<h1>FUCKING WEBSERV</h1>";
	
	if (client.getResponse().getResponseCode() == 200)
		write(client.getClientFD(), s, strlen(s));
	else
	{
		std::string page = gen_html_error_page(client.getResponse().getResponseCode());
		std::string result = "HTTP/1.1 " + SSTR(client.getResponse().getResponseCode()) + " OK\nServer: webserv\nDate: Fri, 04 Feb 2022 03:45:11 GMT\nContent-Type: text/html; charset=utf-8\nContent-Length: " + SSTR(page.size()) + "\nLast-Modified: Tue, 25 Jan 2022 15:41:20 GMT\nConnection: keep-alive\n\n" + page;
		write(client.getClientFD(), result.c_str(), strlen(result.c_str()));
	}
	(void)server;
}