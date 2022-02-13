#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../webserv.hpp"

class Client
{
	public:
		Client();
		Client(std::string _current_server_name, std::string current_server_host, int client_fd, struct sockaddr_in addr);
		Client(Client const &src);
		virtual ~Client();

		Client &	operator=(Client const &src);

		int	&	getClientFD()
		{
			return (this->_client_fd);
		}
		
		long &	getClientTime()
		{
			return (this->_current_time);
		}

		void	setCurrentTime(long current_time)
		{
			this->_current_time = current_time;
		}

		void	setKeepAlive(bool keep_alive)
		{
			this->_keep_alive = keep_alive;
		}

		bool &		isKeepAlive()
		{
			return (this->_keep_alive);
		}

		Request &	getRequest()
		{
			return (this->_request);
		}

		Response &	getResponse()
		{
			return (this->_response);
		}
	private:
		std::string			_current_server_name;
		std::string			_current_server_host;
		int 				_client_fd;
		struct sockaddr_in	_addr;
		long				_current_time;
		bool				_keep_alive;

		Request				_request;
		Response			_response;

};

#endif