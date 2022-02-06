# ifndef CGI_HPP
# define CGI_HPP

# include "../webserv.hpp"

class CGI
{

    public :

        CGI();
        virtual ~CGI();
        CGI(CGI const &src);
        CGI &   operator=(CGI const &src);


    private :

    
};

#endif