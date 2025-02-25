/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:47:20 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:10:13 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	index;

	if ((s == NULL) || (f == NULL))
		return ;
	index = 0;
	while (*(s + index) != '\0')
	{
		f(index, s + index);
		index++;
	}
}
