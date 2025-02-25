/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 11:22:03 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 12:54:20 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*ptrd;
	const unsigned char	*ptrs;

	ptrd = (unsigned char *)dst;
	ptrs = (const unsigned char *)src;
	if (src < dst)
	{
		while (len--)
			*(ptrd + len) = *(ptrs + len);
	}
	else if (src > dst)
	{
		ft_memcpy(dst, src, len);
	}
	return (dst);
}
