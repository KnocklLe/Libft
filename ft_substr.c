/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 14:45:30 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:25:15 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (i < len)
		dest[i++] = s[start++];
	dest[i] = '\0';
	return (dest);
}
// #include <stdio.h>
// #include <stdlib.h>
// int	main(void)
// {
// 	char	*r;

// 	r = ft_substr("Hello World", 6, 5);
// 	printf("test 1 (Hello World, 6, 5) -> ft: [%s] 
// 		| attendu: [World]\n", r);
// 	free(r);
// 	r = ft_substr("Hello", 10, 3);
// 	printf("test 2 (Hello, 10, 3)      -> ft: [%s] 
// 		| attendu: []\n", r);
// 	free(r);
// 	return (0);
// }

