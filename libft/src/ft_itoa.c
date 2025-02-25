/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:46:28 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 12:14:21 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"



char	*ft_itoa(int nb)
{
	char	*ia;
	int		i;
	int		j;

	i = ft_intlen(nb);
	ia = (char *)malloc((i + 1) * sizeof(char));
	if (ia == NULL)
		return (NULL);
	if (nb < 0)
	{
		(*ia++) = (char)('-');
		i--;
	}
	while (i)
	{
		j = (nb / power_10(i - 1)) % 10;
		if (j < 0)
			j = -j;
		*(ia++) = (char)(j + '0');
		i--;
	}
	*ia = '\0';
	return (ia - ft_intlen(nb));
}
