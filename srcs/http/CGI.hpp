# ifndef CGI_HPP
# define CGI_HPP

# include "../webserv.hpp"

/*
     https://httpd.apache.org/docs/2.4/fr/howto/cgi.html 
     https://web.mit.edu/wwwdev/www/cgic.html
     https://www.ietf.org/rfc/rfc3875
*/

class CGI{

    public :

        CGI();
        virtual ~CGI();
        CGI(CGI const &src);
};

#endif 