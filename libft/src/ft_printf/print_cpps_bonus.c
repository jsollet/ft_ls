/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cpps.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:50:04 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 17:01:20 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

void	print_s(t_pgrm *result)
{
	char	*s;
	int		depth;
	int		depth1;

	s = va_arg(result->args, char *);
	if (s == NULL)
		s = "(null)";
	result->n.count = ft_strlen_c1(s, '\0');
	if (result->last.offset_p && result->n.count >= result->last.prec)
		depth = result->last.prec;
	else
		depth = result->n.count;
	depth1 = depth;
	if (result->flag.minus)
	{
		while (depth--)
			ft_putchar_i(*s++, result);
		ft_nputchar_i(result->last.width - depth1, ' ', result);
	}
	else
	{
		print_s0(result);
		while (depth--)
			ft_putchar_i(*s++, result);
	}
}

void	print_c(t_pgrm *result)
{
	char	i;

	i = va_arg(result->args, int);
	result->n.conversion[0] = i;
	result->n.conversion[1] = '\0';
	result->n.count = 1;
	result->sign = 0;
	result->n.nspace = result->last.width - 1;
	if (result->flag.minus)
	{
		ft_putchar_i(i, result);
		if (result->n.nspace >= 0)
			ft_nputchar_i(result->n.nspace, ' ', result);
	}
	else
	{
		if (result->n.nspace >= 0 && result->flag.zero)
			ft_nputchar_i(result->n.nspace, '0', result);
		else
			ft_nputchar_i(result->n.nspace, ' ', result);
		ft_putchar_i(i, result);
	}
}

void	print_pp(t_pgrm *result)
{
	ft_putchar_i('%', result);
}
