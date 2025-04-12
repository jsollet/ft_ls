/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:21:12 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 13:25:09 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/libft.h"
#include "../../includes/libftprintf_fd.h"

int	ft_printf_fd(int fd, const char *inst, ...)
{
	t_pgrm	conversion;

	conversion.count = 0;
	conversion.fd = fd;
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