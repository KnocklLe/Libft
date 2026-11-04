/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memchr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 09:41:02 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/04 10:25:04 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (((unsigned char *)s)[i] && i < n)
	{
		if (((unsigned char *)s)[i] == c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
#include <stdio.h>

	const char str[] = "Welcome to India";
	const char ch = 't';

	char *result = (char *)ft_memchr(str, ch, 2);

	if (result != NULL)
	{
		printf("Trouvé à partir de : %s\n", result);
	}
	else
	{
		printf("Caractère non trouvé.\n");
	}
	return (0);
}*/