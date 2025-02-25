/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:43:02 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 12:55:57 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
