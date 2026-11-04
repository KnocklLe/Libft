/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 12:45:48 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/03 12:51:30 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	bzero(void *s, size_t n)
{
	char	*ptr;

	ptr = s;
	while (*ptr && n--)
		*ptr++ = '0';
}
/*
int	main(void)
{
	#include <stdio.h>
	char str[50] = "Hello, World!";
	size_t n = 5;

	printf("Before bzero: %s\n", str);
	bzero(str, n);
	printf("After bzero: %s\n", str);

	return (0);
}*/