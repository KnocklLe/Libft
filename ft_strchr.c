/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 08:38:50 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/04 09:00:35 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strchr(char *s, int c)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (s[len])
		len++;
	if (len == 0)
		return (NULL);
	while (s[i])
	{
		if (s[i] == c)
			return (&s[i]);
		i++;
	}
	return (s);
}
/*int	main(void)
{
#include <stdio.h>
#include <string.h>

	printf("%s\n", ft_strchr("", 108));
	printf("-----------------------------\n");
	printf("%s\n", strchr("", 108));
}*/