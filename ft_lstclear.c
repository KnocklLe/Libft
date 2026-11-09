/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 08:11:31 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 08:33:37 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list *node;
	t_list *next;
	if (lst == NULL)
		return ;
	while (*lst != NULL)
	{
		node = (*lst);
		next = node->next;
		*lst = next;
		ft_lstdelone(node, del);
	}
}