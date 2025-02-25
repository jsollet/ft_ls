/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:47:00 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:14:44 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	index;
	size_t			len;
	char			*result;

	if ((s == NULL) || (f == NULL))
		return (NULL);
	index = 0;
	len = ft_strlen(s);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	while (index < len)
	{
		*(result + index) = f(index,*(s + index));
		index++;
	}
	*(result + index) = '\0';
	return (result);
}
