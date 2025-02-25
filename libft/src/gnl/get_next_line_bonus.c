/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:50:37 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/09 15:30:38 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_bonus.h"

int	init(t_gnl *s, int fd, char *tbr)
{
	s->iter = 0;
	s->fdh = -1;
	s->index = -1;
	s->svn = 0;
	s->buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (s->buffer == NULL)
		return (0);
	if (BUFFER_SIZE < 1 || fd < 0 || read(fd, s->buffer, 0) == -1)
	{
		free(s->buffer);
		return (0);
	}
	while ((tbr != NULL) && (s->fdh != 1))
	{
		ft_memcpy(s->h, tbr + s->svn * (BUFFER_SIZE + 6), 6);
		head(s, 1);
		if (s->fdh == 1)
			break ;
		if (s->fdh == s->fd)
			s->index = s->svn;
		s->svn++;
	}
	if (s->index == -1)
		s->svn = s->svn + 1;
	return (1);
}

void	rebuild(t_gnl *s, t_sv *t)
{
	if (t == NULL || s->fdh == 1)
	{
		s->rc = read(s->fd, s->buffer, BUFFER_SIZE);
		*(s->buffer + s->rc) = '\0';
	}
	else
	{
		while (*(t[s->index].sv + s->c) != '\0')
		{
			*(s->buffer + s->c) = *(t[s->index].sv + s->c);
			s->c++;
		}
		s->rc = read(s->fd, s->buffer + s->c, BUFFER_SIZE - s->c);
		*(s->buffer + s->rc + s->c) = '\0';
		s->rc = s->rc + s->c;
		s->c = 0;
	}
}

void	get_stat_var(t_gnl *s, t_sv *t)
{
	int		i;

	if (s->index >= 0)
		i = s->index;
	else
		i = s->svn - 1;
	if (s->eof && ((s->c % BUFFER_SIZE) != 0) && s->rc - 1 != 0)
	{
		ft_memcpy(t[i].sv, s->buffer + s->c, s->rc - 1);
		ft_memcpy(t[i].sv + s->rc - 1, "\0", 1);
		t[i].fd = s->fd;
		s->tbr_n = s->svn;
	}
	else
	{
		t[i].fd = -1;
		ft_memcpy(t[i].sv, "\0", 1);
		s->tbr_n = s->svn - 1;
	}
}

void	get_line(t_gnl *s, t_sv *t)
{
	s->eof = 0;
	while (s->rc)
	{
		if (*(s->buffer + s->c) != '\n')
		{
			s->rc--;
			s->c++;
		}
		else
		{
			s->c++;
			s->eof = s->c;
			break ;
		}
		if (s->rc == 0)
		{
			s->buffer = addalloc(s->buffer, s->c, s->c + BUFFER_SIZE);
			s->rc = read(s->fd, s->buffer + s->c, BUFFER_SIZE);
		}
	}
	get_stat_var(s, t);
	s->buffer = addalloc(s->buffer, s->c, s->c + 1);
	*(s->buffer + s->c) = '\0';
}

char	*get_next_line(int fd)
{
	t_gnl			s;
	static char		*tbr;
	t_sv			*sv;

	s.fd = fd;
	s.bs = BUFFER_SIZE;
	if (init(&s, fd, tbr) == 0)
		return (NULL);
	sv = malloc(sizeof(t_sv) * s.svn);
	if (sv == NULL)
		return (NULL);
	s.c = 0;
	get_back(&s, sv, tbr);
	get_line(&s, sv);
	tbr = update(&s, sv, tbr);
	if (s.buffer[0] == '\0')
	{
		free(s.buffer);
		free(sv);
		s.buffer = 0;
		sv = 0;
		return (0);
	}
	free(sv);
	return (s.buffer);
}
