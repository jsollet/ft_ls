/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 11:04:42 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:24:37 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>


typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;
// serie ajout
int		power_10(int expo);
size_t	ft_intlen(long long int n);
int 	ft_strcmp(const char *s1, const char *s2);

// serie is...(5)
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
// serie to...(3)
int		ft_tolower(int c);
int		ft_toupper(int c);
int		ft_atoi(const char *src);
// serie str...(8)
size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char	*dst, const	char *src, size_t size);
size_t	ft_strlcat(char	*dst, const	char *src, size_t size);
char	*ft_strchr(const char *src, int c);
char	*ft_strrchr(const char *src, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char	*source, const char *searched, size_t n);
char	*ft_strdup(const char *src);
// serie mem et calloc...(7)
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *str, size_t nfold);
void	*ft_memchr(const void *source, int searched, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *source, size_t nfold);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memset(void *source, int setvalue, size_t nfold);
// serie supplementaire...(11)
char	*ft_substr(char const *src, unsigned int index, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
// serie bonus...(9)
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **alst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **alst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
#endif
