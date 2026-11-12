/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 09:41:02 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:33 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*str;

	i = 0;
	str = (const unsigned char *)s;
	while (i < n)
	{
		if (str[i] == (unsigned char)c)
			return ((void *)str + i);
		i++;
	}
	return (NULL);
}
// #include <stdio.h>
// #include <string.h>
// int	main(void)
// {
// 	char	*s = "Hello";

// 	printf("test 1 'l', n=5 -> ft: %s | sys: %s\n", 
// 		(char *)ft_memchr(s, 'l', 5), (char *)memchr(s, 'l', 5));
// 	printf("test 2 'o', n=2 -> ft: %p 
// 		| sys: %p\n", ft_memchr(s, 'o', 2), memchr(s, 'o', 2));
// 	return (0);
// }