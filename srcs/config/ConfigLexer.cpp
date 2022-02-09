#include "../webserv.hpp"

ConfigLexer::ConfigLexer() : tokens() {}

ConfigLexer::ConfigLexer(std::string const &src) : raw_string(src), tokens(this->parse(this->raw_string)) {}

ConfigLexer::ConfigLexer(ConfigLexer const &src)
{
	*this = src;
}

ConfigLexer::~ConfigLexer() {}

ConfigLexer	&	ConfigLexer::operator=(ConfigLexer const &src)
{
	this->raw_string = src.raw_string;
	this->tokens = src.tokens;
	return (*this);
}

bool	ConfigLexer::is_space(char c) const
{
	switch (c)
	{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			return true;
		default:
			return false;
	}
}

bool	ConfigLexer::is_word_letter(char c) const
{
  switch (c) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '_':
	case '/':
	case '\\':
	case '$':
	case '.':
	case ':':
	case ',':
      return true;
    default:
      return false;
  }
}

std::string	ConfigLexer::get_token_name_from_type(TokenType token_type) const
{
	switch (token_type)
	{
		case Word:
			return ("Word");
		case Space:
			return ("Space");
		case LeftCurly:
			return ("Left Curly");
		case RightCurly:
			return ("Right Curly");
		case Semicolon:
			return ("Semicolon");
		default:
			return ("Error");
	}
}

TokenType	ConfigLexer::get_token_type(char c) const
{
	if (c == ';')
		return (Semicolon);
	if (c == '{')
		return (RightCurly);
	if (c == '}')
		return (LeftCurly);
	if (this->is_space(c))
		return (Space);
	if (this->is_word_letter(c))
		return (Word);
	return (Error);
}

std::vector<ConfigLexer::Token>	ConfigLexer::parse(std::string raw)
{
	std::vector<ConfigLexer::Token> tokens;

	if (raw.empty())
		throw ConfigLexerException("Configuration parse failed", "Raw string is empty");
	if (!this->recursive_parse(raw, tokens, 0))
		throw ConfigLexerException("Configuration parse failed", "Unauthorized character");
	return (tokens);
}

bool	ConfigLexer::has_exceed_len_limit(TokenType token_type, int len) const
{
	switch(token_type)
	{
		case Semicolon:
		case LeftCurly:
		case RightCurly:
			return (len >= 1);
		default:
			return (false);
	}
}

bool	ConfigLexer::recursive_parse(std::string raw, std::vector<ConfigLexer::Token> &tokens, size_t index)
{
	TokenType original_token_type = this->get_token_type(raw[index]);
	TokenType current_token_type;
	size_t len = 1;

	while (!has_exceed_len_limit(original_token_type, len) && original_token_type == (current_token_type = this->get_token_type(raw[index + len])) && index + len < raw.size())
		len++;
	if (current_token_type == Error && index + len < raw.size())
		return (false);
	if (original_token_type != Space)
		tokens.push_back(ConfigLexer::Token(raw.substr(index, len), original_token_type));
	if (index + len >= raw.length())
		return (true);
	return (recursive_parse(raw, tokens, index + len));
}

std::vector<ConfigLexer::Token>	ConfigLexer::getTokens()
{
	return (this->tokens);
}