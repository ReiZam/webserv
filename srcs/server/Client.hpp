#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <webserv.hpp>

class Client
{
	public:
		Client();
		Client(int client_fd, struct sockaddr_in addr);
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

		bool &							isKeepAlive()
		{
			return (this->_keep_alive);
		}

		Request &						getRequest()
		{
			return (this->_request);
		}

		Response &						getResponse()
		{
			return (this->_response);
		}

		std::vector<unsigned char> &	getBinaryRequest()
		{
			return (this->_binary_request);
		}

		std::string &					getStringRequest()
		{
			return (this->_string_request);
		}

		int								getErrorCounter()
		{
			return (this->_error_counter);
		}

		void							resetErrorCounter()
		{
			this->_error_counter = 0;
		}

		void							incrementErrorCounter()
		{
			this->_error_counter++;
		}
		
		std::string								getRequestBody();
		std::string								getRequestHeader();
		std::vector<unsigned char>::iterator	getBodyBegin();
		std::string 							getIp() const;

		void		reset_client();
		bool		_read();
		bool		_write();
	private:
		std::string					_current_server_name;
		std::string					_current_server_host;
		int 						_client_fd;
		struct sockaddr_in			_addr;
		long						_current_time;
		bool						_keep_alive;
		int							_error_counter;

		std::vector<unsigned char>	_binary_request;
		std::string					_string_request;

		Request						_request;
		Response					_response;

};

#endif
