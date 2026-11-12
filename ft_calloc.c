/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 13:56:54 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:22:04 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	size_t	totalsize;
	char	*dest;

	totalsize = nmemb * size;
	if (size != 0 && nmemb > SIZE_MAX / size)
		return (NULL);
	dest = malloc(sizeof(char) * totalsize);
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (i < totalsize)
	{
		dest[i] = 0;
		i++;
	}
	return (dest);
}
/*
int	main(void)
{
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdint.h>
	int	*a;
	int	*b;

	a = ft_calloc(3, sizeof(int));
	b = calloc(3, sizeof(int));
	printf("test 1 3 int -> ft: %d %d %d 
		| sys: %d %d %d\n", a[0], a[1], a[2], b[0], b[1], b[2]);
	free(a);
	free(b);
	a = ft_calloc(SIZE_MAX, 2);
	printf("test 2 overflow -> ft: %p | attendu: (nil)\n", (void *)a);
	free(a);
	return (0);
}*/