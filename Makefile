# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/21 11:14:30 by tilmanfs          #+#    #+#              #
#    Updated: 2023/09/29 19:28:52 by tfriedri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME			=	ircserv

CPPFLAGS		=	-Wall -Werror -Wextra -std=c++98

SRCS			=	main.cpp	\
					Server.cpp	\
					Channel.cpp	\
					Client.cpp	\
					Message.cpp	\

OBJS			=	$(SRCS:.cpp=.o)

$(NAME):			$(OBJS)
					$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)

all:				$(NAME)

clean:
					rm -rf $(OBJS)

fclean:				clean
					rm -f $(NAME)

re:					fclean all