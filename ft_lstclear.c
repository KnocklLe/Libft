/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 08:11:31 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 13:35:43 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*node;
	t_list	*next;

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
// #include <stdio.h>
// #include <stdlib.h>
// int	main(void)
// {
// 	t_list	*lst;

// 	lst = ft_lstnew(ft_strdup("A"));
// 	lst->next = ft_lstnew(ft_strdup("B"));
// 	ft_lstclear(&lst, free);
// 	printf("test 1 [A B] -> lst: %p | attendu: (nil)\n", (void *)lst);
// 	ft_lstclear(&lst, free);
// 	printf("test 2 liste vide -> lst: %p | attendu: (nil)\n", (void *)lst);
// 	return (0);
// }