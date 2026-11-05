/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 14:18:24 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/05 16:57:24 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*void    ft_test(unsigned int i, char *c)
{
	(void)i;
	*c = *c - 32;
}

void	ft_test2(unsigned int i, char *c)
{
	char			*dest;

	(void)i;
	*c = *c - 1;
}*/

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	i = 0;
    if (!s || !f)
        return ;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*int	main(void)
{
    #include <stdio.h>
    char    str[50] = "test";

	ft_striteri(str, ft_test);
    printf("test : %s\n", str);
    ft_striteri(str, ft_test2);
    printf("test : %s\n", str);
}*/