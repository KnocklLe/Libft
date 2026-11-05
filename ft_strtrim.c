/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 07:41:48 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/05 10:01:54 by edolemai         ###   ########.fr       */
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
static size_t	ft_skip_start(char const *s1, char const *set)
{
	size_t	i;
	size_t	start;

	i = 0;
	start = 0;
	while (s1[i])
	{
		if (ft_is_set(set, s1[i]) == 1)
			start++;
		else
			break ;
		i++;
	}
	return (start);
}
static size_t	ft_skip_end(char const *s1, char const *set)
{
	size_t	i;
	size_t	end;

	end = 0;
	i = ft_strlen(s1) - 1;
	while (s1[i])
	{
		if (ft_is_set(set, s1[i]) == 1)
			end++;
		else
			break ;
		i--;
	}
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*dest;
	size_t	total_size;

	if (set == NULL || s1 == NULL)
		return (NULL);
	start = ft_skip_start(s1, set);
	end = ft_skip_end(s1, set);
	if (start + end >= ft_strlen(s1))
	{
		dest = malloc(1);
		if (dest == NULL)
			return (NULL);
		dest[0] = '\0';
		return (dest);
	}
	total_size = ft_strlen(s1) - (start + end);
	dest = malloc(sizeof(char) * total_size + 1);
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, s1 + start, total_size + 1);
	return (dest);
}
/*int	main(void)
{
#include <stdio.h>

	char *dest;

	dest = ft_strtrim("..test..", "..");
	printf("%s\n", dest);
}*/