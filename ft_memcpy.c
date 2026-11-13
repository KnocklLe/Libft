/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 13:38:03 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/13 08:34:26 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	if (!dest && !src)
		return (dest);
	i = 0;
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
// #include <stdio.h>
// #include <string.h>
// int	main(void)
// {
// 	char	a[10];
// 	char	b[10];
// 	int		src[3] = {1, 2, 3};
// 	int		c[3];
// 	int		d[3];

// 	ft_memcpy(a, "Hello", 6);
// 	memcpy(b, "Hello", 6);
// 	printf("test 1 \"Hello\" -> ft: %s
// | sys: %s\n", a, b);
// 	ft_memcpy(c, src, sizeof(src));
// 	memcpy(d, src, sizeof(src));
// 	printf("test 2 int[3] -> ft: %d %d %d
// 		| sys: %d %d %d\n", c[0], c[1], c[2], d[0], d[1], d[2]);
// 	return (0);
// }