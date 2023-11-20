# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/21 11:14:30 by tilmanfs          #+#    #+#              #
#    Updated: 2023/10/26 11:02:30 by tfriedri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME			=	ircserv

CPPFLAGS		=	-Wall -Werror -Wextra -std=c++98 -g

SRCS			=	srcs/main.cpp	\
					srcs/Server.cpp	\
					srcs/Channel.cpp	\
					srcs/User.cpp	\
					srcs/Message.cpp	\
					srcs/cmmnds/pass.cpp	\
					srcs/cmmnds/nick.cpp	\
					srcs/cmmnds/user.cpp	\
					srcs/cmmnds/quit.cpp	\
					srcs/cmmnds/join.cpp	\
					srcs/cmmnds/ping.cpp	\
					srcs/cmmnds/privmsg.cpp	\
					srcs/cmmnds/notice.cpp	\
					srcs/cmmnds/part.cpp	\
					srcs/cmmnds/topic.cpp	\
					srcs/cmmnds/kick.cpp	\
					srcs/cmmnds/names.cpp	\
					srcs/cmmnds/mode.cpp	\
					srcs/cmmnds/invite.cpp	\
					srcs/cmmnds/list.cpp	\

OBJS			=	$(SRCS:.cpp=.o)

$(NAME):			$(OBJS)
					$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)

all:				$(NAME)

clean:
					rm -rf $(OBJS)

fclean:				clean
					rm -f $(NAME)

re:					fclean all