/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 14:18:24 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:24:10 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
// #include <stdio.h>
// void	f(unsigned int i, char *c)
// {
// 	*c = *c + i;
// }

// int	main(void)
// {
// 	char	s1[] = "aaaa";
// 	char	s2[] = "";

// 	ft_striteri(s1, f);
// 	printf("test 1 (aaaa) -> ft: %s | attendu: abcd\n", s1);
// 	ft_striteri(s2, f);
// 	printf("test 2 (\"\") -> ft: [%s] | attendu: []\n", s2);
// 	return (0);
// }
