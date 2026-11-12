/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 13:51:49 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 13:28:45 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
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
// 	ft_lstadd_front(&lst, b);
// 	printf("test 1 -> %s %s 
// 		| attendu: B A\n", (char *)lst->content, (char *)lst->next->content);
// 	free(a);
// 	free(b);
// 	lst = NULL;
// 	a = ft_lstnew("A");
// 	ft_lstadd_front(&lst, a);
// 	printf("test 2 (liste vide) -> %s 
// 		| attendu: A\n", (char *)lst->content);
// 	free(a);
// 	return (0);
// }