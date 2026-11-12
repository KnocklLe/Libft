/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 14:12:27 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 13:32:51 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_lstsize(t_list *lst)
{
	unsigned int	count;

	count = 0;
	while (lst)
	{
		lst = lst->next;
		count++;
	}
	return (count);
}
// #include <stdio.h>
// #include <stdlib.h>
// int	main(void)
// {
// 	t_list	*a;
// 	t_list	*b;
// 	t_list	*c;

// 	a = ft_lstnew("A");
// 	b = ft_lstnew("B");
// 	c = ft_lstnew("C");
// 	a->next = b;
// 	b->next = c;
// 	printf("test 1 [A B C] -> %d | attendu: 3\n", ft_lstsize(a));
// 	printf("test 2 NULL    -> %d | attendu: 0\n", ft_lstsize(NULL));
// 	free(a);
// 	free(b);
// 	free(c);
// 	return (0);
// }