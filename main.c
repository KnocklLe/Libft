/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 11:28:10 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 08:45:07 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	main(void)
{
	t_list	*test;
	t_list	*lst;
	t_list	*new;
	t_list	*node2;
	t_list	*node3;

	// char	str[5];
	// int		n;
	// *str = "Hello";
	// n = 5;
	// printf("Before bzero: %s\n", str);
	// ft_bzero(str, n);
	// printf("After bzero: %s\n", str);
	// return (0);
	// printf("\n ----> ft_putendl_fd\n");
	// printf("TEST EXIT 1\n");
	// ft_putendl_fd("HELOO", 1);
	// printf("\n");
	// printf("TEST EXIT 2\n");
	// ft_putendl_fd("HELOO", 2);
	// printf("__Test putnbr__\n");
	// ft_putnbr_fd(10, 1);
	// test = ft_lstnew("test ?");
	// printf("%s\n", (char *)test->content);
	// lst = malloc(sizeof(t_list));
	// new = malloc(sizeof(t_list));
	// lst->content = "Bonjour";
	// lst->next = NULL;
	// new->content = "Salut";
	// new->next = NULL;
	// ft_lstadd_front(&lst, new);
	// printf("%s\n", (char *)lst->content);
	// printf("%s\n", (char *)lst->next->content);
	// free(lst);

    lst = malloc(sizeof(t_list));
    node2 = malloc(sizeof(t_list));
    node3 = malloc(sizeof(t_list));
    new = malloc(sizeof(t_list));

    // lst->content = "Bonjour";
    // lst->next = node2;

    // node2->content = "Salut";
    // node2->next = node3;

    // node3->content = "test";
    // node3->next = NULL;

    // new->content = "Nouveau";
    // new->next = NULL;

    // printf("Avant ajout : %u\n", ft_lstsize(lst));
    // printf("Dernier noeud : %s\n",
    //     (char *)ft_lstlast(lst)->content);

    // ft_lstadd_back(&lst, new);

    // printf("\n--- APRES AJOUT ---\n");

    // printf("Taille de la liste : %u\n", ft_lstsize(lst));
    // printf("Dernier noeud : %s\n", (char *)ft_lstlast(lst)->content);

    // free(new);
    // free(node3);
    // free(node2);
    // free(lst);

	// lst = ft_lstnew(ft_strdup("Premier"));
	// node2 = ft_lstnew(ft_strdup("Deuxieme"));
	// node3 = ft_lstnew(ft_strdup("Troisieme"));

	// lst->next = node2;
	// node2->next = node3;

	// printf("Avant suppression :\n");
	// printf("%s\n", (char *)lst->content);
	// printf("%s\n", (char *)node2->content);
	// printf("%s\n", (char *)node3->content);

	// ft_lstdelone(node2, free);

	// printf("\nApres suppression :\n");
	// printf("%s\n", (char *)lst->content);
	// printf("%s\n", (char *)node3->content);

	// free(lst->content);
	// free(lst);
	// free(node3->content);
	// free(node3);

	lst = ft_lstnew(ft_strdup("Premier"));
	lst->next = ft_lstnew(ft_strdup("Deuxieme"));
	lst->next->next = ft_lstnew(ft_strdup("Troisieme"));

	printf("Avant ft_lstclear :\n");
	printf("%s\n", (char *)lst->content);
	printf("%s\n", (char *)lst->next->content);
	printf("%s\n", (char *)lst->next->next->content);

	ft_lstclear(&lst, free);

	printf("\nApres ft_lstclear :\n");
	if (lst == NULL)
		printf("La liste est NULL !\n");
	else
		printf("La liste existe encore.\n");


	return (0);
}
