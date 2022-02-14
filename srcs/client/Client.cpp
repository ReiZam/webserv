#include "../webserv.hpp"

Client::Client() : _current_server_name(), _current_server_host(), _client_fd(-1), _current_time(get_current_time()), _request() {}

Client::Client(std::string current_server_name, std::string current_server_host, int client_fd, struct sockaddr_in addr) : _current_server_name(current_server_name), _current_server_host(current_server_host), _client_fd(client_fd), _addr(addr), _current_time(get_current_time()), _request() {}

Client::Client(Client const &src)
{
	*this = src;
}

Client &	Client::operator=(Client const &src)
{
	this->_current_server_name = src._current_server_name;
	this->_current_server_host = src._current_server_host;
	this->_client_fd = src._client_fd;
	this->_current_time = src._current_time;
	this->_request = src._request;
	// this->_response = src._response;
	return (*this);
}

Client::~Client()
{
	(void)this->_addr;
	if (this->_client_fd != -1)
		close(this->_client_fd);
}