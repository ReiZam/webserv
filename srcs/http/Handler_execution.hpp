# ifndef HANDLER_EXECUTION_HPP
# define HANDLER_EXECUTION_HPP

# include "../webserv.hpp"

/*
     https://httpd.apache.org/docs/2.4/fr/howto/cgi.html 
     https://web.mit.edu/wwwdev/www/cgic.html
     https://www.ietf.org/rfc/rfc3875
*/

class Server;

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
};

#endif 