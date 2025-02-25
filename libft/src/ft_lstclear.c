/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:52:26 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 10:27:17 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static void	free_ll(t_list *start)
{
	if (start)
		free_ll(start->next);
	free(start);
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*start;

	start = *lst;
	while (*lst)
	{
		del((*lst)-> content);
		(*lst) = (*lst)-> next;
	}
	free_ll(start);
	start = NULL;
}
