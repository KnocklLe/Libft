/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 09:02:16 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/04 09:37:33 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	j;
    int len;
    
	i = 0;
	len = 0;
	while (s[len])
		len++;
	if (len == 0)
		return (NULL);
	while (s[i])
	{
		j = i + 1;
		while (s[j])
		{
			if (s[i] == s[j] && s[i] == c)
				i++;
			j++;
		}
		if (s[i] != s[j] && s[i] == c)
			return ((char *)&s[i]);
		i++;
	}
	return ((char *)s);
}
/*
int	main(void)
{
#include <stdio.h>

	printf("%s\n", ft_strrchr("je test la fonction", 101));
	printf("%s\n", strrchr("je test la fonction", 101));
}*/