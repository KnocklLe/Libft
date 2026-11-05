/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 13:47:32 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/05 14:14:24 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*char    ft_test(unsigned int i, char c)
{
    (void)i;
    return(c - 32);
}

char    ft_test2(unsigned int i, char c)
{
    (void)i;
    return(c - 1);
}*/
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int i;
	char *dest;

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
/*
int main (void)
{
    #include <stdio.h>
    
    printf("test 1 :[%s]\n", ft_strmapi("test1", ft_test));
    printf("test 2 :[%s]\n", ft_strmapi("test2", ft_test2));
}*/