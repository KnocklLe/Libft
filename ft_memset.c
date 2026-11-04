/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 12:14:50 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/03 16:33:16 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memset(void *b, int c, size_t len)
{
	if (b == NULL || len <= 0)
		return (NULL);
	while (len--)
		*((unsigned char *)b++) = (unsigned char)c;
	return (b);
}
/*
int	main(void)
{
#include <stdio.h>
#include <string.h>

	{
		char str[50];
		char str2[50];
		int c = 'X';
		size_t n = 0;
		strcpy(str, " ");
		printf("--- Test ft_memset ---\n");
		printf("Avant : %s\n", str);
		ft_memset(str, c, n);
		printf("Apres : %s\n", str);
		printf("------------------\n");
		strcpy(str2, " ");
		printf("--- Test memset ---\n");
		printf("Avant : %s\n", str2);
		memset(str2, c, n);
		printf("Apres : %s\n", str2);
	}
	{
		int tab1[10];
		int tab2[10];
		ft_memset(tab1, 1, sizeof(tab1));
		memset(tab2, 1, sizeof(tab2));

		printf("Test avec 0 :\n");
		printf("ft_memset : %d\n", tab1[0]);
		printf("   memset : %d\n", tab2[0]);

		printf("------------------\n");

		ft_memset(tab1, 255, sizeof(tab1));

		printf("Test avec l'octet 255 :\n");
		printf("Valeur du premier int : %d (doit faire -1)\n", tab1[0]);
	}
	return (0);
}*/