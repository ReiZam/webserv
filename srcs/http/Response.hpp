# ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../webserv.hpp"

class Response{

    public :

        // Constructor 

        Response();
        virtual ~Response();
        Response(Response const &src);
        Response &operator=(Response const &src);

        // Variable 

        std::map<std::string, std::string> _response_header;
        std::pair<int, std::string> _response_status;
        std::string _response_line;
        std::map<std::string, std::string> _response_cgi_hearder;
        std::string _response_cgi_body;
        std::string _response_body;
        std::map<int, std::string> _response_status_table;      

        // Fonction 

        void clear_response(void);
        void status_response(void);
};

# endif
