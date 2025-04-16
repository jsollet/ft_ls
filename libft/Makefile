# Définition des répertoires
SRCDIR = src
PRINTF_SRCDIR = src/ft_printf
GNL_SRCDIR = src/gnl
PRINTF_FD_SRCDIR = src/ft_printf_fd

OBJDIR = obj
PRINTF_OBJDIR = $(OBJDIR)/printf
GNL_OBJDIR = $(OBJDIR)/gnl
PRINTF_FD_OBJDIR = $(OBJDIR)/printf_fd
INCDIR = includes

# Trouver tous les fichiers sources
SRCS = $(wildcard $(SRCDIR)/*.c)
PRINTF_SRCS = $(wildcard $(PRINTF_SRCDIR)/*.c)

GNL_SRCS = $(wildcard $(GNL_SRCDIR)/*.c)

# ajout de printf_fd
PRINTF_FD_SRCS = $(wildcard $(PRINTF_FD_SRCDIR)/*.c)

# Création des objets correspondants
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
PRINTF_OBJS = $(PRINTF_SRCS:$(PRINTF_SRCDIR)/%.c=$(PRINTF_OBJDIR)/%.o)
# Creation objet gnl
GNL_OBJS = $(GNL_SRCS:$(GNL_SRCDIR)/%.c=$(GNL_OBJDIR)/%.o)

PRINTF_FD_OBJS = $(PRINTF_FD_SRCS:$(PRINTF_FD_SRCDIR)/%.c=$(PRINTF_FD_OBJDIR)/%.o)

# Fichier d'en-tête
HEADER = $(INCDIR)/libft.h

# Nom de la bibliothèque
NAME = libft.a

# Commandes
RM = rm -f
MKDIR = mkdir -p
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCDIR) -O3

# Compilation des fichiers sources généraux
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADER) | $(OBJDIR)/
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation des fichiers sources de ft_printf
$(PRINTF_OBJDIR)/%.o: $(PRINTF_SRCDIR)/%.c $(HEADER) | $(PRINTF_OBJDIR)/
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation des fichiers sources de gnl
$(GNL_OBJDIR)/%.o: $(GNL_SRCDIR)/%.c $(HEADER) | $(GNL_OBJDIR)/
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation des fichiers sources de printf_fd
$(PRINTF_FD_OBJDIR)/%.o: $(PRINTF_FD_SRCDIR)/%.c $(HEADER) | $(PRINTF_FD_OBJDIR)/
	$(CC) $(CFLAGS) -c $< -o $@

# Création de la bibliothèque
$(NAME): $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS) $(PRINTF_FD_OBJS)
	ar rc $(NAME) $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS) $(PRINTF_FD_OBJS)
	ranlib $(NAME)

# Nettoyage des fichiers objets
clean:
	$(RM) $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS) $(PRINTF_FD_OBJS)

# Nettoyage complet
fclean: clean
	$(RM) $(NAME)

# Recompiler tout
re: fclean all

# Cible principale
all: $(NAME)

# Définir les cibles .PHONY
.PHONY: all clean fclean re