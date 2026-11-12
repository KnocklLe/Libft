/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 07:41:48 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:48 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_is_set(char const *set, char c)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static size_t	ft_skip(char const *str, char const *set, int reverse)
{
	size_t	i;
	size_t	count;

	count = 0;
	if (!str || !str[0])
		return (0);
	if (reverse)
		i = ft_strlen(str) - 1;
	else
		i = 0;
	while (str[i] && ft_is_set(set, str[i]))
	{
		count++;
		if (reverse && i == 0)
			return (count);
		if (reverse)
			i--;
		else
			i++;
	}
	return (count);
}

char	*ft_strtrim(char const *str, char const *set)
{
	size_t	start;
	size_t	end;
	char	*dest;
	size_t	total_size;
	size_t	s1size;

	if (set == NULL || str == NULL)
		return (NULL);
	s1size = ft_strlen(str);
	start = ft_skip(str, set, 0);
	end = ft_skip(str, set, 1);
	if (start + end >= s1size)
	{
		dest = malloc(1);
		if (dest == NULL)
			return (NULL);
		dest[0] = '\0';
		return (dest);
	}
	total_size = ft_strlen(str) - (start + end);
	dest = malloc(sizeof(char) * total_size + 1);
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, str + start, total_size + 1);
	return (dest);
}
// #include <stdio.h>
// #include <stdlib.h>
// int	main(void)
// {
// 	char	*r;

// 	r = ft_strtrim("  xxHello x Worldxx  ", " x");
// 	printf("test 1 -> ft: [%s] | attendu: [Hello x World]\n", r);
// 	free(r);
// 	r = ft_strtrim("xxxx", "x");
// 	printf("test 2 -> ft: [%s] | attendu: []\n", r);
// 	free(r);
// 	return (0);
// }