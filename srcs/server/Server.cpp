#include "../webserv.hpp"

Server::Server() : _socket_fd(-1), _addr(), _clients() {}

Server::Server(Server const &src)
{
	*this = src;
}

Server::Server(ServerConfig &config) : _config(config), _socket_fd(-1), _addr(), _clients() {}

Server::~Server()
{
	for (std::vector<Client*>::iterator it = this->_clients.begin();it != this->_clients.end();)
		delete (*(it++));
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
	if ((this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw ServerException("socket()", std::string(strerror(errno)));
	if (setsockopt(this->_socket_fd, SOL_SOCKET,  SO_REUSEADDR | SO_REUSEPORT, (char*)&_enable, sizeof(_enable)) < 0)
        throw ServerException("setsockopt()", std::string(strerror(errno)));
	if (bind(this->_socket_fd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0)
        throw ServerException("bind()", std::string(strerror(errno)));
	if (listen(this->_socket_fd, 256) < 0)
        throw ServerException("listen()", std::string(strerror(errno)));
	if (fcntl(this->_socket_fd, F_SETFL, O_NONBLOCK) < 0)
        throw ServerException("fcntl()", std::string(strerror(errno)));

	this->_max_fd = this->_socket_fd;

	std::cout << "[Server] Server listening on " << this->_config.getHost() << "..." << std::endl;
}

void	Server::update_max_fd()
{
	int max_fd = this->_socket_fd;

	for (std::vector<Client*>::iterator it = this->_clients.begin();it != this->_clients.end();it++)
		if ((*it)->getClientFD() > this->_max_fd)
			this->_max_fd = (*it)->getClientFD();

	this->_max_fd = max_fd;
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

	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
		throw ServerException("fcntl()", std::string(strerror(errno)));
	
	FD_SET(client_fd, rset);
	
	std::cout << "[Server] New client (FD: " << client_fd << ") connected to server " << this->_config.getServerName() << " (Host: " << this->_config.getHost() << ")" << std::endl;
}

void	Server::close_client(std::vector<Client*>::iterator &it, fd_set *rset, fd_set *wset)
{
	std::cout << "[Server] Client (FD: " <<  (*it)->getClientFD() << ") has been disconnected from server " << this->_config.getServerName() << " (Host: " << this->_config.getHost() << ")" << std::endl;
	if (FD_ISSET((*it)->getClientFD(), rset))
		FD_CLR((*it)->getClientFD(), rset);
	if (FD_ISSET((*it)->getClientFD(), wset))
		FD_CLR((*it)->getClientFD(), wset);
	delete *it;
	it = this->_clients.erase(it);
	this->update_max_fd();
}

bool	Server::client_request(Client *client)
{
	if (client->_read())
	{
		if (client->getRequest().GetStep() == START)
		{
			client->setCurrentTime(get_current_time());
			std::cout << "[Server] Client (FD: " <<  client->getClientFD() << ") has sent a request to server " << this->_config.getServerName() << " (Host: " << this->_config.getHost() << ")" << std::endl;
		}
		try
		{
			this->_client_handler.handleRequest(*client, *this);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			return (false);
		}
	}
	return (true);
}

bool	Server::client_response(Client *client)
{
	try
	{
		this->_client_handler.handleResponse(*client, *this);
	}
	catch(const std::exception& e)
	{
		std::cout << strerror(errno) << std::endl;
		std::cerr << e.what() << '\n';
		return (false);
	}

	if (client->_write())
	{
		client->setCurrentTime(get_current_time());

		std::cout << "[Server] Client (FD: " <<  client->getClientFD() << ") received response " << client->getResponse().getResponseCode() << " from server " << this->_config.getServerName() << " (Host: " << this->_config.getHost() << ") in " << (get_current_time() - client->getClientTime()) << "ms" << std::endl;
	
		if (client->getRequest().GetErrorCode() == REQUEST_ENTITY_TOO_LARGE)
			return (false);
		return (true);
	}
	return (false);
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
			if (!(*it)->getRequest().isFinished() && !this->client_request(*it))
			{
				this->close_client(it, rset, wset);
				continue ;
			}
			FD_SET((*it)->getClientFD(), wset);
		}
		if (FD_ISSET((*it)->getClientFD(), wset))
		{
			FD_CLR((*it)->getClientFD(), wset);
			if ((*it)->getRequest().isFinished() && !this->client_response(*it))
			{
				this->close_client(it, rset, wset);
				continue ;
			}
		}
		if (!(*it)->isKeepAlive() || get_current_time() - (*it)->getClientTime() > 30 || (*it)->getErrorCounter() >= 5)
			this->close_client(it, rset, wset);
	 	else
		{
			FD_SET((*it)->getClientFD(), rset);
			if ((*it)->getRequest().isFinished())
				(*it)->reset_client();
			++it;
		}
	}
	FD_SET(this->_socket_fd, rset);
}