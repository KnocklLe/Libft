/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 12:14:50 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:35 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t len)
{
	unsigned char	*p;

	p = (unsigned char *)ptr;
	while (len--)
	{
		*p = (unsigned char)value;
		p++;
	}
	return (ptr);
}
// #include <stdio.h>
// #include <string.h>

// int	main(void)
// {
// 	char	a[10] = "Hello 42";
// 	char	b[10] = "Hello 42";
// 	char	c[10] = "Hello 42";
// 	char	d[10] = "Hello 42";

// 	ft_memset(a, 'A', 5);
// 	memset(b, 'A', 5);
// 	printf("test 1 5 x 'A' -> ft: %s | sys: %s\n", a, b);
// 	ft_memset(c, '*', 1);
// 	memset(d, '*', 1);
// 	printf("test 2 1 x '*' -> ft: %s | sys: %s\n", c, d);
// 	return (0);
// }