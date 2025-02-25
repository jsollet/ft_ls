/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftmodb.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:26:01 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 14:13:30 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

int	ft_issp(int c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r' || c == ' ' || c == '.')
		return (1);
	return (0);
}

int	ft_issw(int c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r' || c == ' ' || c == '#'
		|| c == '0' || c == '+' || c == '-')
		return (1);
	return (0);
}

size_t	ft_atoi1(t_funcptr before, const char *str)
{
	int	res;
	int	sign;

	sign = 1;
	res = 0;
	while (before(*str))
	{
		str++;
	}
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while ((*str >= '0') && (*str <= '9' ))
	{
		res = 10 * res + (*str - '0');
		str++;
	}
	return (res * sign);
}
