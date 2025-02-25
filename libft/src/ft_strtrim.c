/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:45:08 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/01 12:07:47 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strtrim(char const *src, char const *set)
{
	char	*dst;
	size_t	lens;
	size_t	i;
	size_t	lend;

	if (src == NULL)
		return (NULL);
	if (set == NULL)
		return ((char *)src);
	lens = ft_strlen(src);
	while ((lens) && ((ft_strchr(set,*(src + lens)))))
		lens--;
	if (lens == 0)
		return (ft_strdup(""));
	i = 0;
	while ((*(src + i) != '\0') && (ft_strchr(set, *(src + i))))
		i++;
	lend = lens - i + 1;
	dst = (char *)malloc((lend + 1) * sizeof(char));
	if (dst == NULL)
		return (NULL);
	*(dst + lend) = '\0';
	while (lend)
		*(dst + --lend) = *(src + lens--);
	return (dst);
}
