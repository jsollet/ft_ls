/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printhelp2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:46:36 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 13:47:36 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

void	left_u(t_pgrm *result)
{
	if (result->last.prec - result->n.count >= 0)
		ft_nputchar_i(result->last.prec - result->n.count, '0', result);
	ft_putstr_i(result->n.conversion, result);
	if (result->n.nspace >= 0)
		ft_nputchar_i(result->n.nspace, ' ', result);
}

static void	if_1(t_pgrm *r)
{
	if (r->flag.zero && r->last.offset_p)
	{
		if (r->n.count > r->last.prec)
			r->delta = r->last.width - (r->n.count + r->sign);
		else
			r->delta = r->last.width - (r->last.prec + r->sign);
		ft_nputchar_i(r->delta, ' ', r);
	}
	else
	{
		if (r->flag.zero && !r->last.offset_p)
		{
			if (r->n.signe == -1)
				ft_putchar_i('-', r);
			else
				ft_printsign(r);
			r->delta = r->last.width - (r->n.count + r->sign);
			ft_nputchar_i(r->delta, '0', r);
			r->done = 1;
		}
	}
}

static void	if_2(t_pgrm *r)
{
	if (!r->flag.zero && r->last.offset_p)
	{
		r->delta = r->last.width - (r->n.count + r->sign);
		ft_nputchar_i(r->delta, ' ', r);
	}
	else
	{
		if (!r->flag.zero && !r->last.offset_p)
		{
			r->delta = r->last.width - (r->n.count + r->sign);
			ft_nputchar_i(r->delta, ' ', r);
		}
	}
}

void	right_ui(t_pgrm *result)
{
	if_1(result);
	if_2(result);
	if (!result->done && result->n.signe == -1)
		ft_putchar_i('-', result);
	else
		ft_printsign(result);
	if (result->flag.zero || result->last.prec > result->last.width)
		ft_nputchar_i(result->last.prec - result->n.count, '0', result);
	ft_putstr_i(result->n.conversion, result);
}

void	left_i(t_pgrm *result)
{
	if (result->n.signe == -1)
		ft_putchar_i('-', result);
	else
		ft_printsign(result);
	if (result->last.prec - result->n.count >= 0)
		ft_nputchar_i(result->last.prec - result->n.count, '0', result);
	ft_putstr_i(result->n.conversion, result);
	if (result->n.nspace >= 0)
		ft_nputchar_i(result->n.nspace, ' ', result);
}
