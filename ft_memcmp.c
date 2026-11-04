/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 10:26:22 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/04 10:53:35 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((((unsigned char *)s1)[i] || ((unsigned char *)s2)[i]) && i < n)
	{
		if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}
/*
int	main(void)
{
#include <stdio.h>

	{
		const char str1[] = "Hello";
		const char str2[] = "every";
		const char str3[] = "World";

		int result1 = ft_memcmp(str1, str2, 5);
		int result2 = ft_memcmp(str1, str3, 5);

		printf("Comparaison de str1 et str2 : %d\n", result1);
		printf("Comparaison de str1 et str3 : %d\n", result2);
	}
	printf("--------------------------------------------------\n");
	{
		const char str1[] = "Hello";
		const char str2[] = "every";
		const char str3[] = "World";

		int result1 = memcmp(str1, str2, 5);
		int result2 = memcmp(str1, str3, 5);

		printf("Comparaison de str1 et str2 : %d\n", result1);
		printf("Comparaison de str1 et str3 : %d\n", result2);
	}
	return (0);
}*/