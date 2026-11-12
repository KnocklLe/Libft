/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 10:29:39 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 13:37:18 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newnode;
	t_list	*newlist;
	void	*contentf;

	newlist = NULL;
	while (lst)
	{
		contentf = f(lst->content);
		newnode = ft_lstnew(contentf);
		if (!newnode)
		{
			del(contentf);
			ft_lstclear(&newlist, del);
			return (NULL);
		}
		ft_lstadd_back(&newlist, newnode);
		lst = lst->next;
	}
	return (newlist);
}

// #include <stdio.h>
// #include <stdlib.h>

// void	*f(void *content)
// {
// 	char	*s;

// 	s = ft_strdup(content);
// 	s[0] = ft_toupper(s[0]);
// 	return (s);
// }

// int	main(void)
// {
// 	t_list	*lst;
// 	t_list	*m;

// 	lst = ft_lstnew(ft_strdup("abc"));
// 	lst->next = ft_lstnew(ft_strdup("def"));
// 	m = ft_lstmap(lst, f, free);
// 	printf("test 1 -> nouvelle: %s %s 
// 		| ancienne: %s %s | attendu: Abc Def | abc def\n",
// 		(char *)m->content, (char *)m->next->content,
// 		(char *)lst->content, (char *)lst->next->content);
// 	ft_lstclear(&m, free);
// 	ft_lstclear(&lst, free);
// 	m = ft_lstmap(NULL, f, free);
// 	printf("test 2 NULL -> %p | attendu: (nil)\n", (void *)m);
// 	return (0);
// }