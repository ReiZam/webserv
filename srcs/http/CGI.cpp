# include "../webserv.hpp"

/* sudo apt-get install php-cgi */

/*
CGI::CGI(){

}

CGI::~CGI(){

}

CGI::CGI(CGI const &src){
    this->_body = src._body;
    this->_response = src._response;
    this->_uri = src._uri;
}

CGI &CGI::operator=(CGI const &src){
    *this = src;
    //(void)src;
    this->_body = src._body;
    this->_response = src._response;
    this->_uri = src._uri;
    return *this;
}

static std::string trim_cgi(std::string const &str)
{
	size_t f, l;
    f = str.find_last_of('.');
    l = str.find_last_of(' ');
    return str.substr(f + 1, (l - f + 1));
}

char **CGI::cgi_env(Client &client, Request &reqt, ServerConfig const &server, std::string &path){
    char **env = 0;
    this->_body = get_file_type(path); 
    std::map<std::string, std::string> _map;    
    int i;


    _map["CONTENT_TYPE"] = client._head.GetValue_cgi("Centent-Type", this->_body);
    //_map["CONTENT_LENGHT"] = std::strtol(server.getBodySize().c_str(), NULL, 10);
    _map["CONTENT_LENGHT"] = server.getBodySize();
    _map["GATEWAY_INTERFACE"] = "CGI/1.1";
    _map["PATH_INFO"] = server.getRoot();
    //_map["PATH_TRANSLATED"] = server.get

    if (reqt.GetMethod() == "GET")
        _map["REQUEST_METHOD"] = "GET";;
    if (reqt.GetMethod() == "POST")
        _map["REQUEST_METHOD"] = "POST";
    //_map["REQUEST_URI"] +=
 
    _map["REMOTE_ADDR"] = inet_ntoa(server.getAddress());
    _map["SERVER_PORT"] = server.getPort();
    //_map["SCRIPT_NAME"] = "." + reqt._uri.ge
    _map["SERVER_NAME"] = server.getServerName();
    _map["REDIRECT_STATUS"] = "200";
    _map["SERVER_SOFTWARE"] = "webserv";

   // find query ('?')
    _map["QUERY_STRING"] = "";    

    std::map<std::string, std::string>::iterator beg = client._head.GetHeader().begin();
    while (beg != client._head.GetHeader().end()){
        _map["HTTP_" + beg->first] = beg->second;
        ++beg;
    }
    env = (char**)malloc(sizeof(char*) * (_map.size() + 1));
    std::map<std::string, std::string>::iterator it = _map.begin();
    i = -1;
    while (it != _map.end()){
        env[++i] = strdup((it->first + "=" + it->second).c_str());
        ++it;
    }
    env[i] = NULL;
    return env;
}

void CGI::parse_cgi(Client &client, Request &reqt, ServerConfig const &server){
     size_t p;
    std::string header, cle, val, status_tmp, buffer;

    buffer = reqt.GetBody();
    if (buffer.find("\r\n\r\n") == std::string::npos)
        return ;
    header = buffer.substr(0, buffer.find("\r\n\r\n") + 1);
    p = header.find("Status");
    if (p != std::string::npos){
        p += 8;
        while (header[p] != ' '){
            status_tmp += header[p];
            p++;
        }
        reqt.setErrorCode(atoi(status_tmp.c_str()));
    }
    p = 0;
    while (header[p]){
        while (header[p] && header[p] != ':'){
            cle += header[p];
            ++p;
        }
        ++p;
        while (header[p] && header[p] != '\r'){
            val += header[p];
            ++p;
        }
        client._head.GetHeader().insert(std::pair<std::string, std::string>(cle, trim_cgi(val)));
        cle.clear();
        val.clear();
        if (header[p] == '\r')
            p++;
        if (header[p] == '\n')
            p++;
    }
    p = buffer.find("\r\n\r\n") + 4;
    reqt.GetBody() = buffer.substr(p);
    cle = "Content-Length";
    val = server.getBodySize(); 
    client._head.GetHeader().insert(std::pair<std::string, std::string>(cle, val));

}

void CGI::exec_cgi(Client &client, Request &reqt, ServerConfig const &server, std::string &path){
    this->_body = get_file_type(path);
    char **ac, **env;
    int r, tab[2];
    std::string result, ext;
    struct stat php;

    ac = (char**)malloc(sizeof(char*) * 3);   
    env = cgi_env(client, reqt, server, path);

    if (ext == ".php"){
        ac[0] = strdup("/usr/bin/php-cgi");
        ac[1] = strdup(server.getRoot().c_str());
    }
    else{
        ac[0] = strdup(server.getRoot().c_str());
        ac[1] = NULL;
    }
    ac[2] = NULL;
    //(void)client, (void)server, (void)reqt;
    

    free(ac), free(env);
}
*/