/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numconverter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:27:56 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 14:15:13 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

static int	power_10(int expo)
{
	int	result;

	result = 1;
	while (expo != 0)
	{
		result *= 10;
		expo--;
	}
	return (result);
}

int	cal(long long int i)
{
	int	k;

	k = i / 4294967296;
	i = i - k * 4294967296;
	while (i >= 2147483648)
		i = i -4294967296;
	while (i < -2147483647)
		i = i + 4294967296;
	return (i);
}

int	calu(long long int i)
{
	int	k;

	k = i / 4294967296;
	i = i - k * 4294967296;
	while (i >= 42949667296)
		i = i - 2 * 2147483648;
	while (i < 0)
		i = i + 2 * 2147483648;
	return (i);
}

void	ft_putnbr_1(int nb, t_n *a)
{
	int	i;
	int	j;

	i = 9;
	if (nb < 0)
		a->signe = -1;
	if (nb)
		while (nb / power_10(i) == 0)
			i--;
	else
		i = 0;
	a->count = i +1;
	while (i > -1)
	{
		j = (nb / power_10(i)) % 10;
		if (j < 0)
			j = -j;
		a->conversion[a->iter++] = j + '0';
		if (a->iter == a->count)
			a->conversion[a->iter++] = '\0';
		i--;
	}
}

void	ft_putnbr_u1(unsigned int nb, t_n *a)
{
	int	i;
	int	j;

	i = 9;
	if (nb)
		while (nb / power_10(i) == 0)
			i--;
	else
		i = 0;
	a->count = i +1;
	while (i > -1)
	{
		j = (nb / power_10(i)) % 10;
		if (j < 0)
			j = -j;
		a->conversion[a->iter++] = j + '0';
		if (a->iter == a->count)
			a->conversion[a->iter++] = '\0';
		i--;
	}
}
