/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:53:22 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 10:53:04 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*lnew;
	t_list	*ltemp;

	if (lst == NULL || f == NULL )
		return (NULL);
	lnew = NULL;
	while (lst)
	{
		ltemp = ft_lstnew((*f)(lst -> content));
		if (ltemp == NULL )
		{
			ft_lstclear(&lnew, del);
			return (NULL);
		}
		ft_lstadd_back(&lnew, ltemp);
		lst = lst -> next;
	}
	return (lnew);
}
