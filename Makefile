NAME			= webserv

SRCS_PATH		= ./srcs/
OBJS_PATH		= ./objs/
OBJS_CFG_PATH	  = ./objs/config
OBJS_HTTP_PATH	= ./objs/http
OBJS_SRV_PATH   = ./objs/server
OBJS_CLT_PATH	= ./objs/client
OBJS_UIS_PATH	= ./objs/utils
OBJS_CGI_PATH	= ./objs/CGI
SRCSC			= webserv.cpp utils/Utils.cpp client/Client.cpp config/Config.cpp config/ConfigLexer.cpp config/LocationConfig.cpp config/ServerConfig.cpp server/Server.cpp http/Handler_execution.cpp CGI/CGI.cpp
# SRCSC			+=	http/Header.cpp http/Uri.cpp http/Request.cpp http/Response.cpp

SRCSH			= 
SRCS			= $(addprefix $(SRCS_PATH),$(SRCSC))

OBJS_NAME		= $(SRCSC:%.cpp=%.o)
OBJS 			= $(addprefix $(OBJS_PATH),$(OBJS_NAME))

LIBS 		= 
CXXFLAGS	= -std=c++98 #-Wall -Wextra -Werror
CXX			= clang++

all:		${NAME}

$(NAME):	${OBJS} ${SRCSH} ${OBJS_TEST}
			@${CXX} ${CXXFLAGS} ${OBJS} ${LIBS} -o ${NAME}
			@echo "\033[1;33m${NAME} program generated."
			@echo "\n"
			@echo "\033[1;32m-------------------------------"
			@echo "\033[1;32m|    ┌─┐┬ ┬┌─┐┌─┐┌─┐┌─┐┌─┐    |"
			@echo 			"|    └─┐│ ││  │  ├┤ └─┐└─┐    |"
			@echo 			"|    └─┘└─┘└─┘└─┘└─┘└─┘└─┘    |"
			@echo "\033[1;32m-------------------------------\033[0m"

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
				@mkdir -p $(OBJS_PATH)
				@mkdir -p $(OBJS_CFG_PATH)
				@mkdir -p $(OBJS_HTTP_PATH)
				@mkdir -p $(OBJS_SRV_PATH)
				@mkdir -p $(OBJS_CLT_PATH)
				@mkdir -p $(OBJS_UIS_PATH)
				@mkdir -p $(OBJS_CGI_PATH)
				@echo "\033[1;30m$@ is generating."
				@clang++ $(CXXFLAGS) -o $@ -c $<
				@echo "\033[1;34m$@ generated."

clean:
			@rm -f ${OBJS}
			@rm -rf ${OBJS_PATH}
			@echo "\033[1;31mRemoving .o files\033[0;0m"

fclean:		clean
			@rm -f ${NAME}
			@echo "\033[1;31mRemoving binary file\033[0;0m"

re:			fclean all

.PHONY:		all clean fclean re