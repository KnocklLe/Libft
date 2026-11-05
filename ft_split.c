/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 10:07:23 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/05 11:14:17 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_is_sep(char const s, char *c)
{
	size_t	i;

	i = 0;
	while (c[i])
	{
		if (c[i] == s)
			return (1);
		i++;
	}
	return (0);
}
static size_t	ft_countwords(void const *s, char *c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (((char *)s)[i])
	{
		if (!ft_is_sep(((char *)s)[i], c) && (i == 0 || ft_is_sep(((char *)s)[i
					- 1], c)))
			count++;
		i++;
	}
	return (count);
}
static char	*ft_wordcpy(void const *s, char *c)
{
	int		i;
	char	*word;
	int		len;

	len = 0;
	i = 0;
	while (((char *)s)[len] && !ft_is_sep(((char *)s)[len], c))
		len++;
	word = malloc(sizeof(char) * len + 1);
	if (word == NULL)
		return (NULL);
	while (((char *)s)[i])
	{
		word[i] = ((char *)s)[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}
char	**ft_split(char const *s, char *c)
{
	int		i;
	int		j;
	char	**dest;

	i = 0;
	j = 0;
	dest = malloc(sizeof(char *) * ft_countwords(s, c));
	if (dest == NULL)
		return (NULL);
	while (s[i] && j < ft_countwords(s, c))
	{
		if (!ft_is_sep(s[i], c))
		{
			dest[j] = ft_wordcpy(&s[i], c);
			if (dest[j] == NULL)
				return (NULL);
			while (!ft_is_sep(s[i], c) && s[i])
				i++;
			j++;
		}
		i++;
	}
	dest[j] = NULL;
	return (dest);
}
/*int	main(void)
{
#include <stdio.h>

	char **tab;
	int i;

	tab = ft_split("salut les,gens", ",");
	while (i < 3)
	{
		printf("[%s]\n", tab[i]);
		i++;
	}
	return (0);
}*/