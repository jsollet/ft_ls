/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printhelp1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:44:14 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 13:47:47 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

void	print_s0(t_pgrm *result)
{
	if (result->n.count < result->last.width)
	{
		if (result->flag.zero)
			ft_nputchar_i(result->last.width - result->last.prec, '0', result);
		else
			ft_nputchar_i(result->last.width - result->n.count, ' ', result);
	}
}

void	space_u(t_pgrm *result)
{
	if (result->last.prec >= result->n.count)
		result->n.nspace = result->last.width - result->last.prec;
	else
		result->n.nspace = result->last.width - result->n.count;
}

void	space_s(t_pgrm *result)
{
	if (result->last.prec >= result->n.count)
		result->n.nspace = result->last.width
			- result->last.prec - result->sign;
	else
		result->n.nspace = result->last.width - result->n.count - result->sign;
}

void	zero(t_pgrm *result)
{
	result->n.count = 0;
	result->n.conversion[0] = '\0';
}
