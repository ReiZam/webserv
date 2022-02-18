#include "../webserv.hpp"

Client::Client() : _current_server_name(), _current_server_host(), _client_fd(-1), _current_time(get_current_time()), _keep_alive(true), _binary_request(), _string_request(), _request(), _response() {}

Client::Client(std::string current_server_name, std::string current_server_host, int client_fd, struct sockaddr_in addr) : _current_server_name(current_server_name), _current_server_host(current_server_host), _client_fd(client_fd), _addr(addr), _current_time(get_current_time()), _keep_alive(true), _binary_request(), _string_request(), _request(), _response() {}

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
	this->_keep_alive = src._keep_alive;
	this->_request = src._request;
	this->_response = src._response;
	return (*this);
}

Client::~Client()
{
	(void)this->_addr;
	if (this->_client_fd != -1)
		close(this->_client_fd);
}

void	Client::reset_client()
{
	this->_request = Request();
	this->_response = Response();
	this->_binary_request.clear();
	this->_string_request.clear();
}

bool	Client::_read()
{
	int	ret;
	unsigned char buffer[BUFFER_SIZE + 1];
	
	while ((ret = read(this->_client_fd, &buffer, BUFFER_SIZE)) > 0)
	{
		this->_string_request.append((char*)buffer, ret);
		for (int i = 0;i < ret;i++)
			this->_binary_request.push_back(buffer[i]);
	}
	if (this->_binary_request.empty())
		return (false);
	return (true);
}

std::string	Client::getRequestHeader()
{
	if (this->_string_request.empty() || this->_string_request.find("\r\n\r\n") == std::string::npos)
		return ("");
	return (this->_string_request.substr(0, this->_string_request.find("\r\n\r\n") + 4));
}

std::string	Client::getRequestBody()
{
	if (this->_string_request.empty() || this->_string_request.find("\r\n\r\n") == std::string::npos)
		return ("");
	return (this->_string_request.substr(this->_string_request.find("\r\n\r\n") + 4));
}

std::vector<unsigned char>::iterator	Client::getBodyBegin()
{
	return (this->_binary_request.begin() + this->_string_request.find("\r\n\r\n") + 4);
}