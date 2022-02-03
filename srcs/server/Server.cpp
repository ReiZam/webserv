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
	(void)src;
	return (*this);
}

void	Server::init()
{
	int _enable = 1;

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