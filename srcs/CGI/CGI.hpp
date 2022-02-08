# ifndef CGI_HPP
# define CGI_HPP

/*
     https://httpd.apache.org/docs/2.4/fr/howto/cgi.html 
     https://web.mit.edu/wwwdev/www/cgic.html
     https://www.ietf.org/rfc/rfc3875
*/


# include "../webserv.hpp"

class CGI
{

    public :

        CGI();
        virtual ~CGI();
        CGI(CGI const &src);
        CGI &   operator=(CGI const &src);
       // void test(Client &client);

    private :

    
};

#endif