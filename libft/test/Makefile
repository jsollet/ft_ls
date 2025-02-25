CC = gcc
CFLAGS = -Wall -Wextra -Werror -I../includes
NAME = test_program

SRCS = main.c
OBJS = $(SRCS:.c=.o)

LIBFT = ../libft.a

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
