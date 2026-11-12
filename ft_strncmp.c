/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 12:05:11 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:42 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
// #include <stdio.h>
// #include <string.h>
// int	main(void)
// {
// 	printf("test 1 (abc, abd, 3) -> ft: %d | sys: %d\n", 
// 		ft_strncmp("abc", "abd", 3), strncmp("abc", "abd", 3));
// 	printf("test 2 (abc, abd, 2) -> ft: %d | sys: %d\n", 
// 		ft_strncmp("abc", "abd", 2), strncmp("abc", "abd", 2));
// 	return (0);
// }