/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:44:14 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 15:30:34 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *src, unsigned int index, size_t len)
{
	char	*dst;
	size_t	lens;
	size_t	dec;

	if (src == NULL)
		return (NULL);
	lens = ft_strlen(src);
	if (index > lens)
		return (ft_strdup(""));
	if (len <= (lens - index))
		dec = len;
	else
		dec = lens - index;
	dst = (char *)malloc((dec + 1) * sizeof(char));
	if (dst == NULL)
		return (NULL);
	*(dst + dec) = '\0';
	while (dec)
	{
		dec--;
		*(dst + dec) = *(src + index + dec);
	}
	return (dst);
}
