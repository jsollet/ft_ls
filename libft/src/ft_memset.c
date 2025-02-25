/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:00:39 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 12:55:08 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *source, int setvalue, size_t nfold)
{
	unsigned char	*ptr1;
	unsigned char	c;
	size_t			size;

	ptr1 = source;
	c = (unsigned char)setvalue;
	size = nfold;
	while (size != 0)
	{
		*ptr1++ = c;
		size--;
	}
	return (source);
}
