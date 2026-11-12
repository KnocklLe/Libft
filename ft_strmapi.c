/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 13:47:32 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:24:24 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*dest;

	i = 0;
	if (!s || !f)
		return (NULL);
	while (s[i])
		i++;
	dest = malloc(sizeof(char) * (i + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dest[i] = f(i, s[i]);
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
// #include <stdio.h>
// #include <stdlib.h>
// char	f(unsigned int i, char c)
// {
// 	return (c + i);
// }

// int	main(void)
// {
// 	char	*r;

// 	r = ft_strmapi("aaaa", f);
// 	printf("test 1 (aaaa) -> ft: %s | attendu: abcd\n", r);
// 	free(r);
// 	r = ft_strmapi("", f);
// 	printf("test 2 (\"\") -> ft: [%s] | attendu: []\n", r);
// 	free(r);
// 	return (0);
// }
