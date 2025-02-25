/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 13:36:43 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 12:09:44 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t nfold)
{
	unsigned char	*ptr;

	ptr = str;
	while (nfold)
	{
		*ptr++ = 0;
		nfold--;
	}
}
