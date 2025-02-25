/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 16:33:09 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:12:07 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char	*dst, const	char *src, size_t size)
{
	size_t	len_dst;
	size_t	len_src;
	size_t	tbc;
	size_t	dst_index;

	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	dst_index = len_dst;
	tbc = 0;
	if (size <= len_dst)
	{
		return (len_src + size);
	}
	else
	{
		while ((*(src + tbc) != '\0') && tbc < (size - len_dst - 1))
		{
			*(dst + dst_index) = *(src + tbc);
			dst_index++;
			tbc++;
		}
		*(dst + dst_index) = '\0';
	}
	return (len_dst + len_src);
}
