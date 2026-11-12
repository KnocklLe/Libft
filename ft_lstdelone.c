/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 07:52:13 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 13:37:48 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst == NULL)
		return ;
	del(lst->content);
	free(lst);
}
// #include <stdio.h>
// #include <stdlib.h>
// int	main(void)
// {
// 	t_list	*a;
// 	t_list	*b;

// 	a = ft_lstnew(ft_strdup("A"));
// 	ft_lstdelone(a, free);
// 	printf("test 1 -> pas de crash, pas de fuite (valgrind)\n");
// 	a = ft_lstnew(ft_strdup("A"));
// 	b = ft_lstnew(ft_strdup("B"));
// 	a->next = b;
// 	ft_lstdelone(a, free);
// 	printf("test 2 -> B existe encore: %s 
// 		| attendu: B\n", (char *)b->content);
// 	ft_lstdelone(b, free);
// 	return (0);
// }