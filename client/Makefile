
NAME			=	client

SRCS			=	client.cpp

OBJS			=	$(SRCS:.cpp=.o)

$(NAME):			$(OBJS)
					$(CXX) $(SRCS) -o client -pthread -lreadline

all:				$(NAME)

clean:
					rm -rf $(OBJS)

fclean:				clean
					rm -f $(NAME)

re:					fclean all