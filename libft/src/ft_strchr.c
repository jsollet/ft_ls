/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 10:26:23 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:07:52 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *source, int searched)
{
	char		*ptr2;

	ptr2 = (char *)source;
	while ((*ptr2 != '\0') && (*ptr2 != (char)searched))
		ptr2++;
	if ((*ptr2 == '\0') && (*ptr2 == (char)searched))
		return (ptr2);
	if (*ptr2 == '\0')
	{
		return (NULL);
	}
	else
	{
		return (ptr2);
	}	
}
