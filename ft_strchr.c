/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 08:38:50 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:38 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}
// #include <stdio.h>
// #include <string.h>
// int	main(void)
// {
// 	char	*s = "Hello World";

// 	printf("test 1 'o'  -> ft: %s 
// 		| sys: %s\n", ft_strchr(s, 'o'), strchr(s, 'o'));
// 	printf("test 2 '\\0' -> ft: %p 
// 		| sys: %p\n", ft_strchr(s, '\0'), strchr(s, '\0'));
// 	return (0);
// }