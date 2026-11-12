/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 08:46:21 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 13:36:18 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst == NULL)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
// #include <stdio.h>
// #include <stdlib.h>

// void	f(void *content)
// {
// 	char	*s;

// 	s = content;
// 	s[0] = ft_toupper(s[0]);
// }

// int	main(void)
// {
// 	t_list	*lst;

// 	lst = ft_lstnew(ft_strdup("abc"));
// 	lst->next = ft_lstnew(ft_strdup("def"));
// 	ft_lstiter(lst, f);
// 	printf("test 1 -> %s %s | attendu: Abc Def\n", 
// 		(char *)lst->content, (char *)lst->next->content);
// 	ft_lstclear(&lst, free);
// 	ft_lstiter(NULL, f);
// 	printf("test 2 NULL -> pas de crash\n");
// 	return (0);
// }