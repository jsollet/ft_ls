/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 13:54:16 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:16:29 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char	*haystack, const char *needle, size_t len)
{
	size_t		i;
	size_t		j;

	if (ft_strlen(needle) == 0)
		return ((char *)haystack);
	i = 0;
	while ((*haystack != '\0') && (i < len))
	{	
		j = 0;
		if (*(haystack + i) == *(needle + j))
		{
			while ((*(haystack + i + j) == *(needle + j)) && (i + j) < len)
			{
				j++;
				if (*(needle + j) == '\0')
					return ((char *)(haystack + i));
			}
		}
		i++;
	}
	return (NULL);
}
