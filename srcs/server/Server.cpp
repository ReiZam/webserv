#include "../webserv.hpp"

Server::Server() : _socket_fd(-1), _addr(), _clients() {}

Server::Server(Server const &src)
{
	*this = src;
}

Server::Server(ServerConfig &config) : _config(config), _socket_fd(-1), _addr(), _clients() {}

Server::~Server()
{
}

Server &	Server::operator=(Server const &src)
{
	this->_addr = src._addr;
	this->_clients = src._clients;
	this->_config = src._config;
	this->_max_fd = src._max_fd;
	this->_socket_fd = src._socket_fd;
	return (*this);
}

void	Server::init()
{
	int _enable = 1;

	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET; 
	this->_addr.sin_port = htons(this->_config.getPort());
	this->_addr.sin_addr = this->_config.getAddress();
	if ((this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw ServerException("socket()", std::string(strerror(errno)));
	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&_enable, sizeof(_enable)) < 0)
        throw ServerException("setsockopt()", std::string(strerror(errno)));
	if (bind(this->_socket_fd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) == -1)
        throw ServerException("bind()", std::string(strerror(errno)));
	if (listen(this->_socket_fd, 256) == -1)
        throw ServerException("listen()", std::string(strerror(errno)));
	if (fcntl(this->_socket_fd, F_SETFL, O_NONBLOCK) == -1)
        throw ServerException("fcntl()", std::string(strerror(errno)));	

	this->_max_fd = this->_socket_fd;

	std::cout << "[Server] Server listening on " << this->_config.getHost() << "..." << std::endl;
}

void	Server::accept_client(fd_set *rset)
{
	int	client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	memset(&client_addr, 0, client_len);
	if ((client_fd = accept(this->_socket_fd, (struct sockaddr*)&client_addr, &client_len)) == -1)
		throw ServerException("select()", std::string(strerror(errno)));
	if (client_fd > this->_max_fd)
		this->_max_fd = client_fd;
	getsockname(client_fd, (struct sockaddr*)&client_addr, &client_len);

	this->_clients.push_back(new Client(this->_config.getServerName(), this->_config.getHost(), client_fd, client_addr));

	fcntl(client_fd, F_SETFL, O_NONBLOCK);
   	FD_SET(client_fd, rset);

	std::cout << "[Server] New client connected to server " << this->_config.getServerName() << " (Host: " << this->_config.getHost() << ")" << std::endl;
}

void	Server::close_client(std::vector<Client*>::iterator &it)
{
	Client *client = *it;
	
	it = this->_clients.erase(it);
	delete client;
	std::cout << "[Server] Client disconnected from server " << this->_config.getServerName() << " (Host: " << this->_config.getHost() << ")" << std::endl;
}

bool	Server::client_request(Client *client)
{
	std::string http_request = read_fd(client->getClientFD());

	client->setCurrentTime(get_current_time());
	if (check_http_req_end(http_request))
	{
		client->getRequest().ParseRequest(http_request);
		client->getResponse().setResponseCode(client->getRequest().GetErrorCode());
	}
	else
		client->getResponse().setResponseCode(400);
	return (true);
}

bool	Server::client_response(Client *client)
{
	const char s[] = "HTTP/1.1 200 OK\nServer: webserv\nDate: Fri, 04 Feb 2022 03:45:11 GMT\nContent-Type: text/html; charset=utf-8\nContent-Length: 24\nLast-Modified: Tue, 25 Jan 2022 15:41:20 GMT\nConnection: keep-alive\n\n<h1>FUCKING WEBSERV</h1>";
	
	const char s2[] = "HTTP/1.1 400 OK\nServer: webserv\nDate: Fri, 04 Feb 2022 03:45:11 GMT\nContent-Type: text/html; charset=utf-8\nContent-Length: 24\nLast-Modified: Tue, 25 Jan 2022 15:41:20 GMT\nConnection: keep-alive\n\n<h1>400 bad request</h1>";
	if (client->getResponse().getResponseCode() == 200)
		write(client->getClientFD(), s, strlen(s));
	else
		write(client->getClientFD(), s2, strlen(s2));
	(void)client;
	return (true);
}

void	Server::run(fd_set *rset, fd_set *wset)
{
	if (FD_ISSET(this->_socket_fd, rset))
	{
		try
		{
			this->accept_client(rset);
		}
		catch(std::exception const &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	for (std::vector<Client*>::iterator it = this->_clients.begin();it != this->_clients.end();)
	{
		if (FD_ISSET((*it)->getClientFD(), rset))
		{
			if (!this->client_request(*it))
				continue ;
			FD_SET((*it)->getClientFD(), wset);
		}
		if (FD_ISSET((*it)->getClientFD(), wset))
			if (!this->client_response(*it))
				continue ;
		if (get_current_time() - (*it)->getClientTime() > 2)
			this->close_client(it);
		else
			++it;
	}

	FD_SET(this->_socket_fd, rset);
}