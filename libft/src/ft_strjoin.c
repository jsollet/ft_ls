/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 11:09:34 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 15:29:17 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strjoin(char const *src1, char const *src2)
{
	char	*dst;
	size_t	len1;
	size_t	len2;

	if (src1 == NULL || src2 == NULL)
		return (NULL);
	len1 = ft_strlen(src1);
	len2 = ft_strlen(src2);
	dst = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (dst == NULL)
		return (NULL);
	*(dst + len1 + len2) = '\0';
	while (len2)
	{
		len2--;
		*(dst + len1 + len2) = *(src2 + len2);
	}
	while (len1)
	{
		len1--;
		*(dst + len1) = *(src1 + len1);
	}
	return (dst);
}
