#ifndef SERVER_HPP
# define SERVER_HPP

#include <webserv.hpp>

class Client;
class ClientHandler;

class Server
{
	public:
		Server();
		Server(std::vector<ServerConfig> &config, int port, std::string host, struct in_addr address);
		Server(Server const &src);
		virtual ~Server();

		Server &		operator=(Server const &src);

		void						init();
		void						run(fd_set *rset, fd_set *wset);
		void						accept_client(fd_set *rset);
		void						close_client(std::vector<Client*>::iterator &it, fd_set *rset, fd_set *wset);
		bool						client_request(Client *client);
		bool						client_response(Client *client);

		std::vector<ServerConfig> &				getConfigs()
		{
			return (this->_configs);
		}

		std::vector<ServerConfig> const &		getConfigs() const
		{
			return (this->_configs);
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

		void						update_max_fd();
		class ServerException : public std::exception
        {
            public:
                explicit ServerException(const char *error_category, const char *error_text)
                {
					this->error.append("[ServerException] ");
					this->error.append(error_category);
					this->error.append(": ");
					this->error.append(error_text);
                }

                virtual ~ServerException() throw() {};

                virtual const char *what() const throw(){
                    return (this->error.c_str());
                };
            private:
                std::string error;
        };
	private:
		std::vector<ServerConfig>	_configs;
		int							_port;
		std::string					_host;
		struct in_addr  			_address;
		int							_socket_fd;
		int							_max_fd;
		struct sockaddr_in			_addr;

		std::vector<Client*>		_clients;
		ClientHandler				_client_handler;
};

#endif
