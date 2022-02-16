# ifndef CGI_HPP
# define CGI_HPP

# include "../webserv.hpp"

/*
     https://httpd.apache.org/docs/2.4/fr/howto/cgi.html 
     https://web.mit.edu/wwwdev/www/cgic.html
     https://www.ietf.org/rfc/rfc3875   
*/

class Client;

class Server;

class BlockConfig;


class CGI{

    public :

        CGI();
        virtual ~CGI();
        CGI(CGI const &src);
        CGI &operator=(CGI const &src);
        char **cgi_env(Client &client, Request &reqt, ServerConfig const &server, std::string &path);
        void parse_cgi(Client &client, Request &reqt, ServerConfig const &server);
        void exec_cgi(Client &client, Request &reqt, ServerConfig const &server, std::string &path);   


    private :

        std::string _body;
        Uri _uri;
        std::string _path;
        Response _response;

};

#endif 
