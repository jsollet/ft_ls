/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:35:49 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 13:43:52 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

void	ft_countflag(const char *inst, t_pgrm *fs)
{
	int	len;

	if (fs->last.offset_w == 0 && fs->last.offset_p == 0)
	len = fs->last.offset_c;
	if (fs->last.offset_w == 0 && fs->last.offset_p != 0)
	len = fs->last.offset_p;
	if (fs->last.offset_w != 0)
	len = fs->last.offset_w;
	while (len)
	{
		if (*(inst + len) == '+')
			fs->flag.plus++;
		if (*(inst + len) == '-')
			fs->flag.minus++;
		if (*(inst + len) == '0')
			fs->flag.zero++;
		if (*(inst + len) == '#')
			fs->flag.sharp++;
		if (*(inst + len) == ' ')
			fs->flag.space++;
	len--;
	}
}

void	ft_getwidth(const char *inst, t_pgrm *result)
{
	size_t	wl;

	result->last.width = ft_atoi1(ft_issw, inst);
	wl = ft_intlen(result->last.width);
	if (result->last.offset_p != 0)
		result->last.offset_w = result->last.offset_p - wl;
	if (result->last.offset_p == 0)
		result->last.offset_w = result->last.offset_c - wl;
}

void	ft_clean(t_pgrm *result)
{
	if (result->flag.plus)
		result->flag.space = 0;
	if (result->flag.minus)
		result->flag.zero = 0;
	if (result->flag.sharp)
	{
		result->flag.plus = 0;
		result->flag.space = 0;
	}
}

void	select_arg(t_pgrm *result)
{
	if (result->last.conv == 'c')
		print_c(result);
	if (result->last.conv == 'd')
		print_id(result);
	if (result->last.conv == 'i')
		print_id(result);
	if (result->last.conv == 's')
		print_s(result);
	if (result->last.conv == 'u')
		print_u(result);
	if (result->last.conv == 'x')
		print_x(result);
	if (result->last.conv == 'X')
		print_ux(result);
	if (result->last.conv == 'p')
		print_p(result);
	if (result->last.conv == '%')
		print_pp(result);
}
