/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 09:02:16 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:47 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	while (i >= 0)
	{
		if ((unsigned char)s[i] == (unsigned char)c)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
// #include <stdio.h>
// #include <string.h>
// int	main(void)
// {
// 	char	*s = "Hello World";

// 	printf("test 1 'o' -> ft: %s | sys: %s\n", 
// 		ft_strrchr(s, 'o'), strrchr(s, 'o'));
// 	printf("test 2 'z' -> ft: %p | sys: %p\n", 
// 		ft_strrchr(s, 'z'), strrchr(s, 'z'));
// 	return (0);
// }