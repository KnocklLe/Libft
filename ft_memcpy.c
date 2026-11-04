/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 13:38:03 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/03 16:33:13 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	if (src == NULL || n <= 0)
		return (NULL);
	while (n-- > 0)
		*((unsigned char *)dest++) = *((const unsigned char *)src++);
	*((unsigned char *)dest) = '\0';
	return (dest);
}
/*
int	main(void)
{
	#include <stdio.h>
	{
		char str[50];
		char str2[50];
		char src[50] = "test";
		size_t n = 0;
		strcpy(str, " ");
		printf("--- Test ft_memcpy ---\n");
		printf("Avant : %s\n", str);
		ft_memcpy(str, src, n);
		printf("Apres : %s\n", str);
		printf("------------------\n");
		strcpy(str2, " ");
		printf("--- Test memcpy ---\n");
		printf("Avant : %s\n", str2);
		memcpy(str2, src, n);
		printf("Apres : %s\n", str2);
	}
}*/