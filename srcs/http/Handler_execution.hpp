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

        /* execution server method */

        Handler_execution();
        virtual ~Handler_execution();
        Handler_execution(Handler_execution const &src);
        Handler_execution & operator=(Handler_execution const &src);
        void execution_request(Client &client, Server &server);
        
        /* content type webserver */

        void set_date(Client &client);
        void last_date_modified(Client &client);
        void name_server(Client &client, Server &server);
        void language_server(Client &client);
        void transfer_encoding(Client &client);
        void method_allowed(Client &client, Server &server);


        /* fucking CGI */

        void parse_cgi(Client &client, Server &server);
        char **cgi_env(Client &client, Server &server);
        void exec_cgi(Client &client, Server &server);
};

#endif 