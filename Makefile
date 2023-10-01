# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/21 11:14:30 by tilmanfs          #+#    #+#              #
#    Updated: 2023/10/01 15:23:47 by tfriedri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME			=	ircserv

CPPFLAGS		=	-Wall -Werror -Wextra -std=c++98

SRCS			=	srcs/main.cpp	\
					srcs/Server.cpp	\
					srcs/Channel.cpp	\
					srcs/Client.cpp	\
					srcs/Message.cpp	\
					srcs/utils.cpp	\

OBJS			=	$(SRCS:.cpp=.o)

$(NAME):			$(OBJS)
					$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)

all:				$(NAME)

clean:
					rm -rf $(OBJS)

fclean:				clean
					rm -f $(NAME)

re:					fclean all