/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 11:55:35 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:23:25 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		len;
	char	*dest;
	int		i;

	i = 0;
	len = 0;
	while (s[len])
		len++;
	dest = malloc(sizeof(char) * len + 1);
	if (dest == NULL)
		return (NULL);
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// int	main(void)
// {
// 	char	*a;
// 	char	*b;

// 	a = ft_strdup("Hello 42");
// 	b = strdup("Hello 42");
// 	printf("test 1 \"Hello 42\" -> ft: %s | sys: %s\n", a, b);
// 	free(a);
// 	free(b);
// 	a = ft_strdup("");
// 	b = strdup("");
// 	printf("test 2 \"\" -> ft: [%s] | sys: [%s]\n", a, b);
// 	free(a);
// 	free(b);
// 	return (0);
// }