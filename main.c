/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 11:28:10 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 11:47:52 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* TEST 1 : ENTIER */

void	*add_one(void *content)
{
	int	*new_content;

	new_content = malloc(sizeof(int));
	if (!new_content)
		return (NULL);
	*new_content = *(int *)content + 1;
	return (new_content);
}

/* TEST 2 : CHAINE DE CARACTERES */

void	*to_upper_copy(void *content)
{
	char	*str;
	char	*newstr;
	int		i;

	str = (char *)content;
	newstr = malloc(ft_strlen(str) + 1);
	if (!newstr)
		return (NULL);
	i = 0;
	while (str[i])
	{
		newstr[i] = (char)toupper((unsigned char)str[i]);
		i++;
	}
	newstr[i] = '\0';
	return (newstr);
}

/* FONCTIONS AUXILIAIRES */

void	del_content(void *content)
{
	free(content);
}

void	print_int(void *content)
{
	printf("%d -> ", *(int *)content);
}

void	print_string(void *content)
{
	printf("%s -> ", (char *)content);
}

void	do_nothing(void *content)
{
	(void)content;
}

/* MAIN */

int	main(void)
{
	t_list	*lst;
	t_list	*newlist;
	int		a;
	int		b;
	int		c;

	/* TEST 1 : FT_LSTMAP AVEC DES ENTIERS */

	printf("========== TEST 1 : INTEGERS ==========\n\n");

	a = 1;
	b = 2;
	c = 3;
	lst = NULL;
	ft_lstadd_back(&lst, ft_lstnew(&a));
	ft_lstadd_back(&lst, ft_lstnew(&b));
	ft_lstadd_back(&lst, ft_lstnew(&c));

	printf("Liste originale :\n");
	ft_lstiter(lst, print_int);
	printf("NULL\n");

	newlist = ft_lstmap(lst, add_one, del_content);

	printf("\nNouvelle liste :\n");
	ft_lstiter(newlist, print_int);
	printf("NULL\n");

	printf("\nListe originale apres ft_lstmap :\n");
	ft_lstiter(lst, print_int);
	printf("NULL\n");

	ft_lstclear(&newlist, del_content);
	ft_lstclear(&lst, do_nothing);

	/* TEST 2 : FT_LSTMAP AVEC DES STRINGS */

	printf("\n========== TEST 2 : STRINGS ==========\n\n");

	lst = NULL;
	ft_lstadd_back(&lst, ft_lstnew(ft_strdup("bonjour")));
	ft_lstadd_back(&lst, ft_lstnew(ft_strdup("salut")));
	ft_lstadd_back(&lst, ft_lstnew(ft_strdup("hello")));
	ft_lstadd_back(&lst, ft_lstnew(ft_strdup("libft")));

	printf("Liste originale :\n");
	ft_lstiter(lst, print_string);
	printf("NULL\n");

	newlist = ft_lstmap(lst, to_upper_copy, del_content);

	printf("\nNouvelle liste :\n");
	ft_lstiter(newlist, print_string);
	printf("NULL\n");

	printf("\nListe originale apres ft_lstmap :\n");
	ft_lstiter(lst, print_string);
	printf("NULL\n");

	ft_lstclear(&newlist, del_content);
	ft_lstclear(&lst, del_content);

	return (0);
}
