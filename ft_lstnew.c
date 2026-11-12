/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 13:29:13 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 13:26:49 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}

// #include <stdio.h>
// #include <stdlib.h>

// int	main(void)
// {
// 	t_list	*n;

// 	n = ft_lstnew("Hello");
// 	printf("test 1 -> content: %s, next: %p | attendu: Hello, (nil)\n", (char *)n->content, (void *)n->next);
// 	free(n);
// 	n = ft_lstnew(NULL);
// 	printf("test 2 -> content: %p, next: %p | attendu: (nil), (nil)\n", n->content, (void *)n->next);
// 	free(n);
// 	return (0);
// }