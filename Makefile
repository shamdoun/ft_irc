srcs = main.cpp Server/Server.cpp Client/Client.cpp Socket.cpp treat_commands.cpp Server/Server_utils.cpp \
		Commands/priv_msg.cpp Commands/Channel.cpp Commands/join.cpp Commands/Mode.cpp
objs = $(srcs:.cpp=.o)
cc = g++
flags = -Werror -Wall -Wextra -std=c++98
headers = include/Server.hpp include/Client.hpp include/Socket.hpp include/Channel.hpp
name = irc

%.o : %.cpp
	${cc} $(flags) -c $< -o $@

all : ${name}

${name} : ${objs} ${headers}
	${cc} ${flags} ${objs} -o ${name}

clean:
	rm -f ${objs}

fclean: clean
	rm -f ${name}

re: fclean all
