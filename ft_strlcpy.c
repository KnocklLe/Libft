/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 08:17:54 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:41 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	i = 0;
	while (src[i])
		i++;
	return (i);
}
// #include <stdio.h>
// int	main(void)
// {
// 	char	dst[10];
// 	size_t	ret;

// 	ret = ft_strlcpy(dst, "Hello", 10);
// 	printf("test 1 size 10 -> ret: %zu, dst: %s
// | attendu: 5, Hello\n", ret, dst);
// 	ret = ft_strlcpy(dst, "Hello", 3);
// 	printf("test 2 size 3  -> ret: %zu, dst: %s
// | attendu: 5, He\n", ret, dst);
// 	return (0);
// }