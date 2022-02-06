# ifndef HANDLER_EXECUTION_HPP
# define HANDLER_EXECUTION_HPP

# include "../webserv.hpp"

class Server;

class Client;

class Handler_execution{

    public :

        Handler_execution();
        virtual ~Handler_execution();
        Handler_execution(Handler_execution const &src);
        Handler_execution & operator=(Handler_execution const &src);
        void execution_request(Client &client, Server &server);
        void set_date(Client &client);
        void last_date_modified(Client &client);
        void name_server(Client &client, Server &server);

    private :

};

#endif 