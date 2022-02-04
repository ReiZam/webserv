class Client
{
	public:
		Client();
		Client(std::string _current_server_name, std::string current_server_host, int client_fd, struct sockaddr_in addr);
		Client(Client const &src);
		virtual ~Client();


		int	&	getClientFD()
		{
			return (this->_client_fd);
		}

		int &	getWriteFD()
		{
			return (this->_write_fd);
		}

		int &	getReadFD()
		{
			return (this->_read_fd);
		}
	private:
		std::string			_current_server_name;
		std::string			_current_server_host;
		int 				_client_fd;
		struct sockaddr_in	_addr;
		int					_read_fd;
		int					_write_fd;
		int
};