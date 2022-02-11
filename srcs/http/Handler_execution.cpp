# include "../webserv.hpp"

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

/*
Handler_execution &Handler_execution::operator=(Handler_execution const &src)
{
    *this = src;
    return *this;
}
*/

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
    client._resp._response_header.insert(std::pair<std::string, std::string>("date", buffer));
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
     //   stat(client._request.get_conf()["error_page"].c_str(), &info);
    //strptime(std::to_string(info.st_mtime).c_str(), "%s", &tps);
    tps = localtime(&info.st_mtim.tv_sec);
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T GMT+02", tps); 

    client._resp._response_header.insert(std::pair<std::string, std::string>("Last-Modified", buffer));
    //client._response._r_header.insert(std::pair<std::string, std::string>("Last-Modified", buffer));
}

void Handler_execution::name_server(Client &client, Server &server)
{
    client._resp._response_header.insert(std::pair<std::string, std::string>("server", server.getConfig().getServerName()));    
}

void Handler_execution::language_server(Client &client)
{
    client._resp._response_header.insert(std::pair<std::string, std::string>("Accept-Language", "en"));   
}

void Handler_execution::transfer_encoding(Client &client)
{
    client._resp._response_header.insert(std::pair<std::string, std::string>("Transfer-Encoding", "chunked"));
}


void Handler_execution::method_allowed(Client &client, Server &server)
{
    void(client), void(server);
   // std::string str = server.getConfig().
}


void Handler_execution::method_location(Client &client)
{
    std::string location = NULL; // pr le moment

    // request location 
    client._resp._response_header.insert(std::pair<std::string, std::string>("location", location));
}

void content_type(Client &client)
{
    std::string content, result, ext; //, extension[7], type[7];
    int i = -1;

    // request path
    std::string extension[7] = {"html", "css", "js", "xml", "json", "xml", "x-www-form-urlencoded"};
    std::string type[7] = {"text/html", "text/css", "text/javascript", "text/xml", "application/json", "application/xml", "application/x-www-form-urlencoded"};
    size_t position = content.rfind(".");
    if (position != std::string::npos)
    {
        ext = content.substr(position + 1);
        while (++i < 7)
        {
            if (ext ==  extension[i])
            {
                result = type[i];
                break ;
            }
        }
    }
    if (result.length() == 0)
        result = "text/plain"; // text/html
    client._resp._response_header.insert(std::pair<std::string, std::string>("Content-type", result));
}

char **Handler_execution::cgi_env(Client &client, Server &server)
{   
    char **env = NULL;
    void(client), void(server);

    return env;
}