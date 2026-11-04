/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 16:39:49 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/04 08:16:35 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int	ft_strlen(const char *str)
{
	int	length;

	length = 0;
	while (str[length] != '\0')
	{
		length++;
	}
	return (length);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	int	i;

	i = 0;
	if (dest < src || dest >= src + n)
	{
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((const unsigned char *)src)[i];
			i++;
		}
	}
	else
	{
		i = n - 1;
		while (i != 0)
		{
			((unsigned char *)dest)[i] = ((const unsigned char *)src)[i];
			i--;
		}
	}
	return (dest);
}
/*
int	main(void)
{
	#include <stdio.h>

	char src[50] = "test";
	char dest[50] = "testee";
	char src2[50] = "test";
	char dest2[50] = "testee";

	ft_memmove(dest, src, 10);
	memmove(dest2, src2, 10);
	printf("%s\n", dest);
	printf("%s\n", dest2);
}*/