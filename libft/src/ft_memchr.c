/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 13:40:21 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 12:51:27 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *source, int searched, size_t n)
{
	unsigned char	*ptr1;
	unsigned char	c;
	size_t			size;

	size = n;
	c = (unsigned char)searched;
	ptr1 = (unsigned char *)source;
	while ((size != 0) && (*ptr1 != c))
	{
		ptr1++;
		size--;
	}
	if (size == 0)
	{
		return (NULL);
	}
	else
	{
		return (ptr1);
	}
}
