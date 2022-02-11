# ifndef CLIENT_HPP
# define CLIENT_HPP

class Client
{
	public:
		Client();
		Client(std::string _current_server_name, std::string current_server_host, int client_fd, struct sockaddr_in addr);
		Client(Client const &src);
		virtual ~Client();

		// Class name
		Response _resp;

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
	private:
		std::string			_current_server_name;
		std::string			_current_server_host;
		int 				_client_fd;
		struct sockaddr_in	_addr;
		long				_current_time;
};

#endif