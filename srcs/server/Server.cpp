#include "../webserv.hpp"

Server::Server() : _socket_fd(-1), _addr(), _clients() {}

Server::Server(Server const &src)
{
	*this = src;
}

Server::Server(ServerConfig &config) : _config(config), _socket_fd(-1), _addr(), _clients() {}

Server::~Server()
{
	if (this->_socket_fd != -1)
		close(this->_socket_fd);
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
	if ((client_fd = accept(this->_socket_fd, (struct sockaddr*)&client_addr, &client_len)) < 0)
		throw ServerException("accept()", std::string(strerror(errno)));
	if (client_fd > this->_max_fd)
		this->_max_fd = client_fd;
	getsockname(client_fd, (struct sockaddr*)&client_addr, &client_len);

	this->_clients.push_back(new Client(this->_config.getServerName(), this->_config.getHost(), client_fd, client_addr));

	fcntl(client_fd, F_SETFL, O_NONBLOCK);
   	FD_SET(client_fd, rset);

	std::cout << "[Server] New client connection to server " << this->_config.getServerName() << " (Host: " << this->_config.getHost() << ")" << std::endl;
}

void	Server::close_client(Client *client)
{
	(void)client;
}

bool	Server::read_request(Client *client)
{
	(void)client;
	return (true);
}

bool	Server::write_request(Client *client)
{
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

	for (std::vector<Client*>::iterator it = this->_clients.begin();it != this->_clients.end();it++)
	{
		if (FD_ISSET((*it)->getClientFD(), rset))
			if (!this->request(*it))
				continue ;
		if (FD_ISSET((*it)->getClientFD(), wset))
			if (!this->response(*it))
				continue ;
		if ((*it)->getWriteFD() != -1)
			if (FD_ISSET((*it)->getWriteFD(), wset))

	}
}