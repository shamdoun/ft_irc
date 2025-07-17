srcs = main.cpp Server/Server.cpp Client/Client.cpp Socket.cpp treat_commands.cpp Server/Server_utils.cpp \
		Commands/priv_msg.cpp Commands/Channel.cpp Commands/join.cpp Commands/Mode.cpp Commands/kick.cpp Server/Authentification.cpp \
		Commands/Invite.cpp Commands/Topic.cpp
srcs_bonus = bonus/Bot_bonus.cpp bonus/main_bonus.cpp Socket.cpp
objs = $(srcs:.cpp=.o)
objs_bonus = $(srcs_bonus:.cpp=.o)
cc = c++
flags = -Werror -Wall -Wextra -std=c++98 -fsanitize=address
headers = include/Server.hpp include/Client.hpp include/Socket.hpp include/Channel.hpp include/Authentification.hpp include/Server_utils.hpp
headers_bonus = bonus/Bot_bonus.hpp include/numericReplies.hpp include/Socket.hpp
name = irc
name_bonus = irc_bot

%.o : %.cpp
	${cc} $(flags) -c $< -o $@

all : ${name}

${name} : ${objs} ${headers}
	${cc} ${flags} ${objs} -o ${name}

bonus : ${name_bonus}

${name_bonus} : ${objs_bonus} ${headers_bonus}
	${cc} ${flags} ${objs_bonus} -o ${name_bonus}

clean:
	rm -f ${objs}

fclean: clean
	rm -f ${name}

clean_bonus:
	rm -f ${objs_bonus}

fclean_bonus : clean_bonus
	rm -f ${name_bonus}

re: fclean all

re_bonus: fclean_bonus bonus