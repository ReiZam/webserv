#include <webserv.hpp>

Client::Client() : _client_fd(-1), _current_time(get_current_time()), _keep_alive(true), _error_counter(0), _binary_request(), _string_request(), _request(), _response() {}

Client::Client(int client_fd, struct sockaddr_in addr) : _client_fd(client_fd), _addr(addr), _current_time(get_current_time()), _keep_alive(true), _error_counter(0), _binary_request(), _string_request(), _request(), _response() {}

Client::Client(Client const &src)
{
	*this = src;
}

Client &	Client::operator=(Client const &src)
{
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
	
	while ((ret = recv(this->_client_fd, &buffer, BUFFER_SIZE, MSG_NOSIGNAL)) > 0)
	{
		this->_string_request.append((char*)buffer, ret);
		for (int i = 0;i < ret;i++)
			this->_binary_request.push_back(buffer[i]);
	}
	if (this->_binary_request.empty())
		return (false);
	return (this->_string_request.find("\r\n\r\n") != std::string::npos);
}

bool	Client::_write()
{
	if (send(this->_client_fd, this->_response.getRawHeader().c_str(), this->_response.getRawHeader().size(), 0) < 0)
		return (false);

	std::vector<unsigned char> &body = this->_response.getBody();
	
	for (std::vector<unsigned char>::iterator it = body.begin();it != body.end();it++)
		if (send(this->_client_fd, &(*it), 1, 0) < 0)
			return (false);

	return (true);
}

std::string Client::getIp() const
{
	char tmp[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(this->_addr.sin_addr), tmp, INET_ADDRSTRLEN);

	return (std::string(tmp));
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
