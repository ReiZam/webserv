#ifndef SERVER_HPP
# define SERVER_HPP

#include "../webserv.hpp"

class Server
{
	public:
		Server();
		Server(ServerConfig &config);
		Server(Server const &src);
		virtual ~Server();

		Server &		operator=(Server const &src);

		void						init();
		void						run(fd_set *rset, fd_set *wset);
		void						accept_client(fd_set *rset);
		void						close_client(Client *client);
		bool						request(Client *client);
		bool						response(Client *client);

		ServerConfig &				getConfig()
		{
			return (this->_config);
		}

		int &						getSocketFD()
		{
			return (this->_socket_fd);
		}
		
		int &						getMaxFD()
		{
			return (this->_max_fd);
		}

		std::vector<Client*> &		getClients()
		{
			return (this->_clients);
		}

		struct sockaddr_in const &	getAddr()
		{
			return (this->_addr);
		}

		class ServerException : public std::exception
        {
            public:
                ServerException(std::string error_category, std::string error_text)
                {
                    this->error = "[ServerException] " + error_category +  ": " + error_text;
                }

                virtual ~ServerException() throw() {};

                virtual const char *what() const throw(){
                    return (this->error.c_str());
                };
            private:
                std::string error;
        };
	private:
		ServerConfig			_config;
		int						_socket_fd;
		int						_max_fd;
		struct sockaddr_in		_addr;

		std::vector<Client*>	_clients;
};

#endif