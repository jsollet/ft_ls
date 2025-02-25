/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:49:16 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:01:46 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"



void	ft_putnbr_fd(int nb, int fd)
{
	int	i;
	int	j;

	i = 9;
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
	}
	if (nb)
	{
		while (nb / power_10(i) == 0)
			i--;
	}
	else
		i = 0;
	while (i > -1)
	{
		j = (nb / power_10(i)) % 10;
		if (j < 0)
			j = -j;
		ft_putchar_fd(j + '0', fd);
		i--;
	}
}
