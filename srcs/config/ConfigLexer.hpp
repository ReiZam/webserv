#ifndef CONFIG_LEXER_HPP
# define CONFIG_LEXER_HPP

#include "../webserv.hpp"

enum TokenType
{
	Error = 0,
	Word = 1,
	Space = 2,
	LeftCurly = 3,
	RightCurly = 4,
	Semicolon = 5
};

class ConfigLexer
{
	public:
		ConfigLexer();
		ConfigLexer(ConfigLexer const &src);
		ConfigLexer(std::string const &src);
		virtual ~ConfigLexer();

		ConfigLexer	&	operator=(ConfigLexer const &config);

		class Token
		{
			public:
				Token() {}
				Token(std::string str, TokenType token_type) : str(str), token_type(token_type) {}
				virtual ~Token() {}

				std::string		getString() const
				{
					return (this->str);
				}

				TokenType		getTokenType() const
				{
					return (this->token_type);
				}
			private:
				std::string str;
				TokenType	token_type;
		};

		bool				is_space(char c) const;
		bool				is_word_letter(char c) const;
		bool				has_exceed_len_limit(TokenType token_type, int len) const;
		TokenType			get_token_type(char c) const;
		std::string			get_token_name_from_type(TokenType token_type) const;

		std::vector<Token>	getTokens();

		class ConfigLexerException : public std::exception
        {
            public:
                ConfigLexerException(std::string error_category, std::string error_text)
                {
					this->error = "[ConfigLexerException] " + error_category +  ": " + error_text;
                }

                virtual ~ConfigLexerException() throw() {};
            
                virtual const char *what() const throw(){
                    return (this->error.c_str());
                };
            private:
                std::string error;
        };
	private:
		std::string			raw_string;
		std::vector<Token>	tokens;
		
		void				parse();
		bool				recursive_parse(size_t index);
};

#endif