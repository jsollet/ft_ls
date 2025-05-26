SRCDIR = src
OBJDIR = obj
INCDIR = includes
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a


SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


HEADER = $(INCDIR)

NAME = ft_ls

RM = rm -f
MKDIR = mkdir -p
CC = gcc

CFLAGS = -g  -Wall -Wextra -Werror -I$(INCDIR) -O3
LDFLAGS =


UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LOCAL_INC = local/install/include
	LOCAL_LIB = local/install/lib
    CFLAGS += -I$(LOCAL_INC) -I$(LOCAL_INC)/sys
	LDFLAGS += -L$(LOCAL_LIB) -lacl
    $(info [Makefile] Building on Linux with local ACL/ATTR headers)
else ifeq ($(UNAME_S),Darwin)
    $(info [Makefile] Building on macOS using system headers)
endif

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(LDFLAGS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADER) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(OBJS) 
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME) $(LIBFT)
	$(MAKE) -C $(LIBFT_DIR) fclean
	
re: fclean all

.PHONY: all clean fclean re
