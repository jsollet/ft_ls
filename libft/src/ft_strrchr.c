/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 13:21:47 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:17:15 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *source, int searched)
{
	char	*ptr2;

	ptr2 = (char *)source;
	while (*ptr2 != '\0')
	{
		ptr2++;
	}
	if (*ptr2 == (char)searched)
		return (ptr2);
	while ((ptr2 != source) && (*ptr2 != (char)searched))
	{
		ptr2--;
	}
	if ((ptr2 == source) && (*ptr2 == (char)searched))
		return (ptr2);
	if (ptr2 == source)
	{
		return (NULL);
	}
	else
	{
		return (ptr2);
	}	
}
