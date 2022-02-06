# include "CGI.hpp"

CGI::CGI()
{

}

CGI::~CGI()
{

}

CGI::CGI(CGI const &src)
{
    *this = src;
}

CGI &CGI::operator=(CGI const &src)
{
    *this = src;
    return *this;
}
