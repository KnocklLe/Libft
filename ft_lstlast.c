/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 14:39:48 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 15:43:08 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}
// #include <stdio.h>
// #include <stdlib.h>

// int	main(void)
// {
// 	t_list	*a;
// 	t_list	*b;

// 	a = ft_lstnew("A");
// 	b = ft_lstnew("B");
// 	a->next = b;
// 	printf("test 1 [A B] -> %s | attendu: B\n", (char *)ft_lstlast(a)->content);
// 	printf("test 2 NULL  -> %p | attendu: (nil)\n", (void *)ft_lstlast(NULL));
// 	free(a);
// 	free(b);
// 	return (0);
// }