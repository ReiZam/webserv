# include "../webserv.hpp"

Autoindex::Autoindex(){

}

Autoindex::~Autoindex(){

}

Autoindex::Autoindex(Autoindex const &src){ 
    *this = src;
}

Autoindex &Autoindex::operator=(Autoindex const &src){
    (void)src;
    return *this;
}

/*
static std::string	get_file_content2(std::string const &path)
{
	std::ifstream file(path.c_str());
	std::stringstream content_stream;
	std::string	content;
	
	content_stream << file.rdbuf();
	content = content_stream.str();

	file.close();
	return (content);
}
*/

std::string Autoindex::exec_autoindex(Request &reqt, Client &client){
    std::string result;
    //std::string allPath = reqt.GetUri().AllPath();
    struct dirent *current;
    DIR *dir = opendir(client._request.GetRawPath().c_str());
    //DIR *dir = opendir(reqt.GetUri().AllPath().c_str());
    result += "<html>\n<body>\n";
    result += "<h1> Directory list </h1>\n";
    while ((current = readdir(dir)) != NULL){
        if (current->d_name[0] != '.'){
            result += "<a href=\"" + client._request.GetRawPath();
            if (client._request.GetRawPath() != "/")
                result += "/";
            result += current->d_name;
            result += "\">";
            result += current->d_name;
            result += "</a><br>\n";
        }
    } 
    closedir(dir);
    result += "</body>\n</html>\n";
    return result;
}
