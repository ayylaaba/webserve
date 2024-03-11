NAME = webserve

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

SRCS = 	main.cpp \
		server.cpp \
		location.cpp \
		request.cpp \
		fd_info.cpp \
		get_method.cpp \

OBJ = ${SRCS:.cpp=.o}

all : ${NAME}

${NAME} : ${OBJ}
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean :
	rm -f ${OBJ}

fclean : clean
	rm -f ${NAME}

re : fclean all