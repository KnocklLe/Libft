/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 15:26:02 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 13:34:33 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*lastnode;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		lastnode = ft_lstlast(*lst);
		lastnode->next = new;
	}
}
// #include <stdio.h>
// #include <stdlib.h>

// int	main(void)
// {
// 	t_list	*lst;
// 	t_list	*a;
// 	t_list	*b;

// 	a = ft_lstnew("A");
// 	b = ft_lstnew("B");
// 	lst = a;
// 	ft_lstadd_back(&lst, b);
// 	printf("test 1 -> %s %s 
// | attendu: A B\n", (char *)lst->content, (char *)lst->next->content);
// 	free(a);
// 	free(b);
// 	lst = NULL;
// 	b = ft_lstnew("B");
// 	ft_lstadd_back(&lst, b);
// 	printf("test 2 (liste vide) -> %s 
// | attendu: B\n", (char *)lst->content);
// 	free(b);
// 	return (0);
// }
