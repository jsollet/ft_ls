/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:49:41 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/09 15:29:53 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	*ft_memcpy(void *dest, const void *source, size_t nfold)
{
	unsigned char			*ptrd;
	const unsigned char		*ptrs;

	if (dest == NULL && source == NULL)
		return (NULL);
	ptrs = (const unsigned char *)source;
	ptrd = (unsigned char *)dest;
	while (nfold)
	{
		*ptrd = *ptrs;
		ptrd++;
		ptrs++;
		nfold--;
	}
	return (dest);
}

void	*addalloc(void *ptr, size_t orig_size, size_t size)
{
	void	*p;

	if (orig_size > size)
		return (NULL);
	p = malloc(size);
	if (p == NULL)
		return (NULL);
	p = ft_memcpy(p, ptr, orig_size);
	free(ptr);
	return (p);
}

void	head(t_gnl *s, int c)
{
	if (c == 0)
	{
		s->h[0] = (char)((s->fd / 100000) % 10 + '0');
		s->h[1] = (char)((s->fd / 10000) % 10 + '0');
		s->h[2] = (char)((s->fd / 1000) % 10 + '0');
		s->h[3] = (char)((s->fd / 100) % 10 + '0');
		s->h[4] = (char)((s->fd / 10) % 10 + '0');
		s->h[5] = (char)((s->fd) % 10 + '0');
		s->h[6] = '\0';
	}	
	else
	{
		s->fdh = (s->h[0] - '0') * 100000 + (s->h[1] - '0') * 10000
			+ (s->h[2] - '0') * 1000 + (s->h[3] - '0') * 100
			+ (s->h[4] - '0') * 10 + (s->h[5] - '0');
	}
}

void	*get_back(t_gnl *s, t_sv *t, char *tbr)
{
	long int	i;

	if (tbr != NULL)
	{
		while (s->iter < s->svn)
		{
			i = 0;
			ft_memcpy(s->h, tbr + s->iter * (s->bs + 6), 6);
			head(s, 1);
			t[s->iter].fd = s->fdh;
			while (*(tbr + s->iter *(s->bs + 6) + 6 + i) != '\0')
			{
				t[s->iter].sv[i] = *(tbr + s->iter *(s->bs + 6) + i + 6);
				i++;
			}
			t[s->iter].sv[i] = '\0';
			s->iter++;
		}
		free(tbr);
	}
	if (s->index == -1)
		rebuild(s, NULL);
	else
		rebuild(s, t);
	return (t);
}

void	*update(t_gnl *s, t_sv *t, char *r)
{
	s->i = 0;
	s->j = 0;
	if (s->tbr_n == 0)
		return (NULL);
	else
	{
		r = malloc(sizeof(char) * (7 + s->tbr_n * (s->bs + 6)));
		if (r == NULL)
			return (NULL);
		while (s->i < s->svn)
		{
			if (t[s->i].fd >= 0)
			{
				s->fd = t[s->i].fd;
				head(s, 0);
				ft_memcpy(r + s->j * (s->bs + 6), s->h, 6);
				ft_memcpy(r + 6 * (s->j + 1) + s->j * s->bs, t[s->i].sv, s->bs);
				s->j++;
			}
			s->i++;
		}
		ft_memcpy(r + s->j * (s->bs + 6), "000001", 6);
		*(r + s->j * (s->bs + 6) + 6) = '\0';
	}
	return (r);
}
