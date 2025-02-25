/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:50:07 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/09 15:32:28 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE  100
# endif

typedef struct s_sv
{
	int				fd;
	char			sv[BUFFER_SIZE];
}	t_sv;

typedef struct s_gnl
{
	char			*buffer;
	char			h[7];
	long int		rc;
	long int		fb;
	long int		c;
	long int		bs;
	long int		eof;
	long int		index;
	int				i;
	int				j;
	int				tbr_n;
	int				fd;
	int				fdh;
	int				iter;
	int				svn;
}	t_gnl;

int		init(t_gnl *s, int fd, char *tbr);
void	*addalloc(void *ptr, size_t orig_size, size_t size);
void	rebuild(t_gnl *s, t_sv *t);
void	get_stat_var(t_gnl *s, t_sv *t);
void	get_line(t_gnl *s, t_sv *t);
void	head(t_gnl *s, int c);
void	*get_back(t_gnl *s, t_sv *t, char *tbr);
void	*update(t_gnl *s, t_sv *t, char *tbr);
char	*get_next_line(int fd);
#endif