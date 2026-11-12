/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 16:39:49 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:36 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!(unsigned char *)dest && !(unsigned char *)src)
		return (NULL);
	if ((unsigned char *)dest < (unsigned char *)src
		|| (unsigned char *)dest >= (unsigned char *)src + n)
	{
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((const unsigned char *)src)[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i > 0)
		{
			((unsigned char *)dest)[i - 1] = ((const unsigned char *)src)[i
				- 1];
			i--;
		}
	}
	return (dest);
}
// #include <stdio.h>
// #include <string.h>

// int	main(void)
// {
// 	char	a[10] = "123456789";
// 	char	b[10] = "123456789";
// 	char	c[10] = "123456789";
// 	char	d[10] = "123456789";

// 	ft_memmove(a + 2, a, 5);
// 	memmove(b + 2, b, 5);
// 	printf("test 1 dest > src -> ft: %s | sys: %s\n", a, b);
// 	ft_memmove(c, c + 2, 5);
// 	memmove(d, d + 2, 5);
// 	printf("test 2 dest < src -> ft: %s | sys: %s\n", c, d);
// 	return (0);
// }