/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 11:09:45 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/04 11:40:18 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
    if (!little[0])
        return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] && little[j] && (i + j) < len && big[i + j] == little[j])
			j++;
        if (!little[j])
            return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
#include <stdio.h>

	printf("%s\n", ft_strnstr("je test la fonction", "la", 50));
}*/