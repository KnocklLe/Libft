/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 08:17:54 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/03 11:50:57 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strlcpy(char *dest, const char *src, int size)
{
	int	i;

	if (size <= 0)
		return (0);
	i = 0;
	while (src[i] != '\0' && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (i);
}
/*
int	main(void)
{
	#include <stdio.h>
	char dest[20];
	const char *src = "Hello, World!";
	int size = 20;

	int result = ft_strlcpy(dest, src, size);
	printf("Result: %d\n", result);
	printf("Destination: %s\n", dest);

	return (0);
}*/