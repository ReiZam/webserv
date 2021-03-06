#include <webserv.hpp>

std::string	read_fd(int fd)
{
	std::string result;
	int ret;
	char buffer[BUFFER_SIZE + 1];
	
	while ((ret = read(fd, buffer, BUFFER_SIZE)) > 0)
		result.append(buffer, ret);
	result[result.size()] = 0;
	return (result);
}

bool	check_http_req_end(std::string request)
{
	return (request.size() >= 4 && request.find("\r\n\r\n") != std::string::npos);
}

long	get_current_time()
{
	struct timeval time_c;

    gettimeofday(&time_c, 0);
    return (time_c.tv_sec * 1000 + time_c.tv_usec / 1000);
}

std::vector<std::string> split_vect(std::string const &string, char delimiteur)
{
	std::vector<std::string> str;
    size_t begin, end;

    end = 0;
    
    while ((begin = string.find_first_not_of(delimiteur, end)) != std::string::npos){
        end = string.find(delimiteur, begin);
    str.push_back(string.substr(begin, end - begin));      
    }
    return str;	
}

std::string trim(std::string const &str)
{
	size_t f, l;
    f = str.find_last_of('.');
    l = str.find_last_of(' ');
    return str.substr(f + 1, (l - f + 1));
}

std::string		gen_status_code(int	code)
{
	switch (code)
	{
		case CONTINUE: 
			return "100 Continue";
		case SWITCHING_PROTOCOLS:
			return "101 Switching Protocols";
		case OK:
			return "200 OK";
		case CREATED:
			return "201 Created";
		case ACCEPTED:
			return "202 Accepted";
		case NON_AUTHORITATIVE_INFORMATION:
			return "203 Non-Authoritative Information";
		case NO_CONTENT:
			return "204 No Content";
		case RESET_CONTENT:
			return "205 Reset Content";
		case PARTIAL_CONTENT:
			return "206 Partial Content";
		case MOVED_PERMANENTLY:
			return "301 Moved Permanently";
		case FOUND:
			return "302 Found";
		case SEE_OTHER:
			return "303 See Other";
		case NOT_MODIFIED:
			return "304 Not Modified";
		case USE_PROXY:
			return "305 Use Proxy";
		case SWITCH_PROXY:
			return "306 Switch Proxy";
		case TEMPORARY_REDIRECT:
			return "307 Temporary Redirect";
		case BAD_REQUEST:
			return "400 Bad Request";
		case UNAUTHORIZED:
			return "401 Unauthorized";
		case PAYMENT_REQUIRED:
			return "402 Payment Required";
		case FORBIDDEN:
			return "403 Forbidden";
		case NOT_FOUND:
			return "404 Not Found";
		case METHOD_NOT_ALLOWED:
			return "405 Method Not Allowed";
		case NOT_ACCEPTABLE:
			return "406 Not Acceptable";
		case PROXY_AUTHENTICATION_REQUIRED:
			return "407 Proxy Authentication Required";
		case REQUEST_TIMEOUT:
			return "408 Request Timeout";
		case CONFLICT:
			return "409 Conflict";
		case GONE:
			return "410 Gone";
		case LENGTH_REQUIRED:
			return "411 Length Required";
		case PRECONDITION_FAILED:
			return "412 Precondition Failed";
		case REQUEST_ENTITY_TOO_LARGE:
			return "413 Request Entity Too Large";
		case REQUEST_URI_TOO_LONG:
			return "414 Request-URI Too Long";
		case UNSUPPORTED_MEDIA_TYPE:
			return "415 Unsupported Media Type";
		case REQUESTED_RANGE_NOT_SATISFIABLE:
			return "416 Requested Range Not Satisfiable";
		case EXPECTATION_FAILED:
			return "417 Expectation Failed";
		case INTERNAL_SERVER_ERROR:
			return "500 Internal Server Error";
		case NOT_IMPLEMENTED:
			return "501 Not Implemented";
		case BAD_GATEWAY:
			return "502 Bad Gateway";
		case SERVICE_UNAVAILABLE:
			return "503 Service Unavailable";
		case GATEWAY_TIMEOUT:
			return "504 Gateway Timeout";
		case HTTP_VERSION_NOT_SUPPORTED:
			return "505 HTTP Version Not Supported";
		default:
			return "";
	}
}

std::vector<unsigned char>	string_to_uchar_vec(std::string str)
{
	std::vector<unsigned char> vector;

	for (size_t i = 0;i < str.size();i++)
		vector.push_back((static_cast<unsigned char>(str[i])));
	return (vector);
}

std::vector<unsigned char>	gen_html_error_page(int code)
{
	std::string error_name = gen_status_code(code);
	std::string html_page = "<!DOCTYPE html><html><head><title>" + error_name + "</title><head><body style=\"text-align: center;\"><h1>Webserv (Error)</h1><pre><hr><p>" + error_name + "</p></pre></body></html>";

	return (string_to_uchar_vec(html_page));
}

std::string     GetDate(void)
{
	char		buf[256];
	time_t		rtime = time(&rtime);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", gmtime(&rtime));
	return std::string(buf);
}

std::string     GetLastModifiedDate(const std::string& fpath)
{
	struct stat	result;
	char		buf[256];
	if(stat(fpath.c_str(), &result) == 0)
	{
		strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", gmtime(&result.st_mtime));
		return std::string(buf);
	}
	return std::string("");
}

std::vector<unsigned char> read_file(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);

    file.unsetf(std::ios::skipws);

    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

    std::vector<unsigned char> vector;

    vector.insert(vector.begin(),
               std::istream_iterator<unsigned char>(file),
               std::istream_iterator<unsigned char>());
    return (vector);
}

std::string			rdfile(const char* file)
{
	std::ifstream	stream(file);
	std::string		res((std::istream_iterator<unsigned char>(stream)),
						std::istream_iterator<unsigned char>());
	return res;
}

bool	exist_file(std::string const &path)
{
	struct stat buffer;

	if (stat(path.c_str(), &buffer) != 0)
		return (false);
	return (S_ISREG(buffer.st_mode));
}

bool	exist_directory(std::string const &path)
{
	struct stat buffer;

	if (stat(path.c_str(), &buffer) != 0)
		return (false);
	return (S_ISDIR(buffer.st_mode));
}

bool	ends_with(std::string const &value, std::string const &ending)
{
    if (ending.size() > value.size())
		return (false);
    return (std::equal(ending.rbegin(), ending.rend(), value.rbegin()));
}

bool	starts_with(std::string const &value, std::string const &starts)
{
    if (starts.size() > value.size())
		return (false);
    return (std::equal(starts.begin(), starts.end(), value.begin()));
}

std::string	get_path_ext(std::string path)
{
	if (path.empty() || path.rfind(".") == std::string::npos)
		return ("");
	return (path.substr(path.rfind("."), path.size()));
}

bool	check_ext_path(std::string file_path, std::string extension)
{
	if (file_path.empty() || file_path.size() <= (extension.size() + 1) || !ends_with(file_path, "." + extension))
		return (false);
	return (true);
}

Header	parse_header(std::string header_string)
{
	Header header;
	size_t pos;

	pos = 0;
	while ((pos = header_string.find(":")) != std::string::npos)
	{
		std::string	line = header_string.substr(0, header_string.find("\n"));

		if (line.length() && *(line.end() - 1) == '\n')
			line.resize(line.size() - 1);
		if (line.length() && *(line.end() - 1) == '\r')
			line.resize(line.size() - 1);
		std::string key = (line.empty()) ? "" : line.substr(0, pos);
		std::string value = (line.size() <= pos + 2) ? "" :  line.substr(pos + 1);
		while (value[0] == ' ')
			value.erase(0, 1);
		if (key.empty() || value.empty())
			throw WebservException("Header Parsing", "Invalid header");
		header_string.erase(0, header_string.find("\n") + 1);
		header.SetValue(key, value);
	}
	return (header);
}

std::vector<std::string>	parse_server_name_parts(std::string value)
{
	size_t	current_pos = 0;
	std::vector<std::string> parts;

	while (current_pos != std::string::npos)
	{
		current_pos = value.find(".");
		std::string part = value.substr(0, current_pos);

		parts.push_back(part);
		value = value.substr(current_pos + 1);
	}
	return (parts);
}

bool	check_forbidden_characters_string(std::string string, std::string forbidden_characters)
{
	for (size_t i = 0;i < string.size();i++)
		if (forbidden_characters.find(string[i]) != std::string::npos)
			return (false);
	return (true);
}