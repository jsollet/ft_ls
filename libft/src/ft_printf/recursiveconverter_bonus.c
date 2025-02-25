/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursiveconverter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:30:18 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 13:31:10 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

void	cb_bx(unsigned long long int nb, int base, int depth, t_n *a)
{
	char	*letters;

	a->count++;
	letters = "0123456789ABCDEF";
	if (nb / base && depth -1 > 0)
	{
		depth--;
		cb_bx(nb / base, base, depth, a);
	}
	a->conversion[a->iter++] = letters[nb % base];
	if (a->iter == a->count)
		a->conversion[a->iter++] = '\0';
}

void	cb_lx(unsigned long long int nb, int base, int depth, t_n *a)
{
	char	*letters;

	a->count++;
	letters = "0123456789abcdef";
	if (nb / base && depth -1 > 0)
	{
		depth--;
		cb_lx(nb / base, base, depth, a);
	}
	a->conversion[a->iter++] = letters[nb % base];
	if (a->iter == a->count)
		a->conversion[a->iter++] = '\0';
}
