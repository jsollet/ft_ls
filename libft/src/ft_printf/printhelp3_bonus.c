/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printhelp3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:47:55 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 14:15:47 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void	left_px(t_pgrm *result)
{
	ft_printsign(result);
	if (result->last.prec - result->n.count >= 0)
		ft_nputchar_i(result->last.prec - result->n.count, '0', result);
	ft_putstr_i(result->n.conversion, result);
	if (result->n.nspace >= 0)
		ft_nputchar_i(result->n.nspace, ' ', result);
}

void	right_px(t_pgrm *result)
{
	if (result->last.conv == 'p')
	{
		if (result->n.nspace >= 0)
			ft_nputchar_i(result->n.nspace, ' ', result);
		ft_printsign(result);
	}
	else
	{
		ft_printsign(result);
		if (result->n.nspace >= 0)
		{
			if (result->last.offset_p)
				ft_nputchar_i(result->n.nspace, ' ', result);
			else
			{
				if (result->flag.zero)
					ft_nputchar_i(result->n.nspace, '0', result);
				else
					ft_nputchar_i(result->n.nspace, ' ', result);
			}
		}
		if (result->last.prec - result->n.count >= 0)
			ft_nputchar_i(result->last.prec - result->n.count, '0', result);
	}
	ft_putstr_i(result->n.conversion, result);
}

void	print_nf(char *c, t_pgrm *conversion)
{
	ft_putchar_i(*c, conversion);
	conversion->last.offset_s++;
}

int	ft_printf(const char *inst, ...)
{
	t_pgrm	conversion;

	conversion.count = 0;
	conversion.fd = 1;
	va_start(conversion.args, inst);
	while (*inst != '\0')
	{
		initstruct(ft_strlen_c1(inst, '\0'), &conversion);
		if (*inst != '%')
			print_nf((char *)inst, &conversion);
		else
		{
			ft_lastdelim(inst + 1, "cspdiuxX%", &conversion);
			ft_ptdelim(inst + 1, conversion.last.offset_c, &conversion);
			ft_getwidth(inst + 1, &conversion);
			ft_countflag(inst, &conversion);
			ft_clean(&conversion);
			select_arg(&conversion);
			inst = inst + conversion.last.offset_c;
		}
		inst++;
	}
	va_end(conversion.args);
	return (conversion.count);
}
