#ifndef UTILS_HPP
#define UTILS_HPP

#define BOLD "\e[1m"
#define ITAL "\e[3m"
#define SOUL "\e[4m"
//  COLOR
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YEL "\e[33m"
#define BLUE "\e[34m"
#define MAGE "\e[35m"
#define CYAN "\e[36m"
#define WHITE "\e[37m"
#define LORAN "\e[38;5;222m"
//  DEFAULT & SYSTEM
#define RESET "\e[0m"
#define CLEAR "\e[1;1H\e2J"

#define SHOW_DEBUG( x, c1, c2 )	static_cast<std::string>(x ? c1 : c2)

#endif
