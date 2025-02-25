/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pidux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:53:43 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 17:03:09 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

void	print_u(t_pgrm *result)
{
	unsigned int	u;

	u = va_arg(result->args, unsigned);
	ft_putnbr_u1(calu(u), &result->n);
	if (u == 0 && result->last.offset_p && result->last.prec == 0)
		zero(result);
	space_u(result);
	if (result->flag.minus)
		left_u(result);
	else
		right_ui(result);
}

void	print_id(t_pgrm *result)
{
	int	i;

	i = va_arg(result->args, int);
	ft_putnbr_1(cal(i), &result->n);
	ft_sign(result);
	if (i == 0 && result->last.offset_p && result->last.prec == 0)
		zero(result);
	space_s(result);
	if (result->flag.minus)
		left_i(result);
	else
		right_ui(result);
}

void	print_p(t_pgrm *result)
{
	unsigned long int	p;

	p = va_arg(result->args, unsigned long int);
	cb_lx(p, 16, 16, &result->n);
	ft_sign(result);
	if (p == 0 && result->last.offset_p && result->last.prec == 0)
		zero(result);
	space_s(result);
	if (result->flag.minus)
		left_px(result);
	else
		right_px(result);
}

void	print_x(t_pgrm *result)
{
	unsigned long int	x;

	x = va_arg(result->args, unsigned int);
	cb_lx(cal(x), 16, 8, &result->n);
	ft_sign(result);
	if (x == 0 && result->last.offset_p && result->last.prec == 0)
		zero(result);
	space_s(result);
	if (result->flag.minus)
		left_px(result);
	else
		right_px(result);
}

void	print_ux(t_pgrm *result)
{
	unsigned long int	ux;

	ux = va_arg(result->args, unsigned int);
	cb_bx(cal(ux), 16, 8, &result->n);
	ft_sign(result);
	if (ux == 0 && result->last.offset_p && result->last.prec == 0)
		zero(result);
	space_s(result);
	if (result->flag.minus)
		left_px(result);
	else
		right_px(result);
}
