/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 15:36:25 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:12:56 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char	*dst, const	char *src, size_t size)
{
	size_t	len_src;

	len_src = ft_strlen(src);
	if (size == 0)
		return (len_src);
	while ((size - 1) && (*src != '\0'))
	{
		*dst = *src;
		dst++;
		src++;
		size--;
	}
	*dst = '\0';
	return (len_src);
}
