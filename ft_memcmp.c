/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 10:26:22 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:32 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}
// #include <stdio.h>
// #include <string.h>
// int	main(void)
// {
// 	printf("test 1 (abc, abd, 3)   -> ft: %d | sys: %d\n", 
// 		ft_memcmp("abc", "abd", 3), memcmp("abc", "abd", 3));
// 	printf("test 2 (\\0a, \\0b, 2) -> ft: %d | sys: %d\n", 
// 		ft_memcmp("\0a", "\0b", 2), memcmp("\0a", "\0b", 2));
// 	return (0);
// }