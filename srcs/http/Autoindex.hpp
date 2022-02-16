# ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "../webserv.hpp"

class Client;

//class Response;

class Request;

//class ServerConfig;


class Autoindex{

    public :

        Autoindex();
        virtual ~Autoindex();
        Autoindex(Autoindex const &src);
        Autoindex &operator=(Autoindex const &src);
        std::string exec_autoindex(Request &reqt, Client &client);
};

#endif