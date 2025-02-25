/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftmoda.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:21:12 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 13:25:09 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libftprintf.h"

size_t	ft_strlen_c1(const char *str, char del)
{
	size_t	len;

	len = 0;
	while (*str != '\0' && *str != del)
	{
		str++;
		len++;
	}
	if (del == '\0')
		return (len);
	else
		return (len + 1);
}

size_t	ft_intlen(long long int n)
{
	size_t	result;
	size_t	count;

	result = n;
	count = 0;
	if (n <= 0)
		count++;
	while (result != 0)
	{
		result = result / 10;
		count++;
	}
	return (count);
}

void	ft_putchar_i(char c, t_pgrm *result)
{
	write(result->fd, &c, 1);
	result->count++;
}

void	ft_putstr_i(char *str, t_pgrm *result)
{
	while (*str != '\0')
	{
		write(result->fd, str, 1);
		result->count++;
		str++;
	}
}

void	ft_nputchar_i(int n, char c, t_pgrm *result)
{
	if (n > 0)
	{
		while (n)
		{
			write(result->fd, &c, 1);
			result->count++;
			n--;
		}
	}
}
