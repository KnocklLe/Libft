/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 11:27:38 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:34 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n--)
		*ptr++ = '\0';
}
/*
int	main(void)
{
	#include <strings.h>
	#include <stdio.h>
	char	a[6] = "Hello";
	char	b[6] = "Hello";

	ft_bzero(a, 2);
	bzero(b, 2);
	printf("test 1 a[1] -> ft: %d | sys: %d\n", a[1], b[1]);
	printf("test 2 a[2] (pas touché) -> ft: %c | sys: %c\n", a[2], b[2]);
	return (0);
}*/