# include "Handler_execution.hpp"

Handler_execution::Handler_execution()
{

}

Handler_execution::~Handler_execution()
{

}

Handler_execution::Handler_execution(Handler_execution const &src)
{
    *this = src;
}

Handler_execution &Handler_execution::operator=(Handler_execution const &src)
{
    *this = src;
    return *this;
}

void Handler_execution::execution_request(Client &client, Server &server)
{
    std::string str;
    int request;

    str = "";

    /*
        client status   
    */

   /*
    if (request get)
        get ;
    else if (request post)
        post ;
    else if (request delete)
        delete ;
    */
}

void Handler_execution::set_date(Client &client)
{
    struct timeval _temps;
    struct tm *_time;
    char buffer[64];

    _time = NULL;
    gettimeofday(&_temps, NULL);
    //strptime(std::to_string(_temps.tv_sec).c_str(), "%s", &_time); 
    _time = localtime(&_temps.tv_sec);
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T GMT+02", _time); 
   /*
        insert client
   */
   
   // client._response._r_header.insert(std::pair<std::string, std::string>("date", buffer));
}

void Handler_execution::last_date_modified(Client &client)
{
    struct  stat info;
    struct tm *tps = NULL;
    char buffer[64];
    //std::string c = client._request.get_conf()["path"];
    //stat(c.c_str(), &info);
    if (!S_ISDIR(info.st_mode))
      //  stat(client._request.get_conf()["error_page"].c_str(), &info);
    //strptime(std::to_string(info.st_mtime).c_str(), "%s", &tps);
    tps = localtime(&info.st_mtim.tv_sec);
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T GMT+02", tps); 

    //client._response._r_header.insert(std::pair<std::string, std::string>("Last-Modified", buffer));
}

/*
void Handler_execution::name_server(Client &client, Server &server)
{

}
*/