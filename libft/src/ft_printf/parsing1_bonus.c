/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:31:40 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 13:35:42 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

void	ft_sign(t_pgrm *result)
{
	if (result->flag.sharp && (result->last.conv == 'x'))
		result->sign = 2;
	if (result->flag.sharp && (result->last.conv == 'X'))
		result->sign = 2;
	if (result->flag.plus && result->last.conv == 'd')
		result->sign = 1;
	if (result->flag.plus && result->last.conv == 'i')
		result->sign = 1;
	if (result->flag.space && result->last.conv == 'd')
		result->sign = 1;
	if (result->flag.space && result->last.conv == 'i')
		result->sign = 1;
	if (result->n.signe == -1)
		result->sign = 1;
	if (result->last.conv == 'p')
		result->sign = 2;
}

void	ft_printsign(t_pgrm *result)
{
	if (!(result->n.conversion[0] == '0' && result->n.conversion[1] == '\0'))
	{
		if ((result->flag.sharp != 0) && (result->last.conv == 'x'))
			ft_putstr_i("0x", result);
		if ((result->flag.sharp != 0) && (result->last.conv == 'X'))
			ft_putstr_i("0X", result);
	}
	if ((result->flag.plus != 0) && (result->last.conv == 'd'))
		ft_putstr_i("+", result);
	if ((result->flag.plus != 0) && (result->last.conv == 'i'))
		ft_putstr_i("+", result);
	if ((result->flag.space != 0) && (result->last.conv == 'd'))
		ft_putstr_i(" ", result);
	if ((result->flag.space != 0) && (result->last.conv == 'i'))
		ft_putstr_i(" ", result);
	if (result->last.conv == 'p')
		ft_putstr_i("0x", result);
}

void	ft_lastdelim(const char *inst, const char *code, t_pgrm *result)
{
	if (*inst == '%')
	{
		result->last.conv = '%';
		result->last.offset_c = 1;
	}
	else
	{
		while (*code != '\0')
		{
			if (ft_strlen_c1(inst, *code) < result->last.offset_c)
			{
				result->last.offset_c = ft_strlen_c1(inst, *code);
				result->last.conv = *code;
			}
			code++;
		}
	}
}

void	ft_ptdelim(const char *inst, size_t offset_c, t_pgrm *result)
{
	if (ft_strlen_c1(inst, '.') < offset_c)
	{
		result->last.offset_p = ft_strlen_c1(inst, '.');
		result->last.prec = ft_atoi1(ft_issp, inst + result->last.offset_p);
	}
}

void	initstruct(size_t len, t_pgrm *pgrm)
{
	pgrm->last.offset_s = 0;
	pgrm->last.offset_c = len;
	pgrm->last.offset_p = 0;
	pgrm->last.offset_w = 0;
	pgrm->last.width = 0;
	pgrm->last.prec = 0;
	pgrm->last.conv = '\0';
	pgrm->flag.plus = 0;
	pgrm->flag.minus = 0;
	pgrm->flag.zero = 0;
	pgrm->flag.sharp = 0;
	pgrm->flag.space = 0;
	pgrm->sign = 0;
	pgrm->done = 0;
	pgrm->n.count = 0;
	pgrm->n.signe = 0;
	pgrm->n.iter = 0;
	pgrm->n.nspace = 0;
	pgrm->n.conversion[0] = '\0';
}
