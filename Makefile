SRCDIR = src
OBJDIR = obj
INCDIR = includes #eventuellement les header de ft_ls

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

# Trouver tous les fichiers sources
SRCS = $(wildcard $(SRCDIR)/*.c) $(wildcard sort/*.c) #dernier ajouté, donc que ca a modifier

# Création des objets correspondants
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Fichier d'en-tête
HEADER = $(INCDIR) #a voir

NAME = ft_ls

# Commandes
RM = rm -f
MKDIR = mkdir -p
CC = gcc
CFLAGS = -g  -Wall -Wextra -Werror -I$(INCDIR) #-fsanitize=address

all: $(LIBFT) $(NAME)

# compilation de libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Création de l'exécutable
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

# Compilation des fichiers sources généraux
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADER) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	$(RM) $(OBJS) 
	$(MAKE) -C $(LIBFT_DIR) clean

# Nettoyage complet
fclean: clean
	$(RM) $(NAME) $(LIBFT)
	$(MAKE) -C $(LIBFT_DIR) fclean
	

# Recompiler tout
re: fclean all

# Cible principale
all: $(NAME)

# Définir les cibles .PHONY
.PHONY: all clean fclean re