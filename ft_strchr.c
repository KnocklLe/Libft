/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 08:38:50 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/04 09:49:06 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);

}
/*
int	main(void)
{
#include <stdio.h>
#include <string.h>

	printf("%s\n", ft_strchr("", 101));
	printf("-----------------------------\n");
	printf("%s\n", strchr("", 101));
}*/