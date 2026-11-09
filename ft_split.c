/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 10:07:23 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:37 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_is_sep(char c, char sep)
{
	return (c == sep);
}

static size_t	ft_countwords(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (((char *)s)[i])
	{
		if (!ft_is_sep(s[i], c) && (i == 0 || ft_is_sep(s[i - 1], c)))
			count++;
		i++;
	}
	return (count);
}

static char	*ft_wordcpy(void const *s, char c)
{
	size_t	i;
	char	*word;
	size_t	len;

	len = 0;
	i = 0;
	while (((char *)s)[len] && !ft_is_sep(((char *)s)[len], c))
		len++;
	word = malloc(sizeof(char) * (len + 1));
	if (word == NULL)
		return (NULL);
	while (i < len)
	{
		word[i] = ((char *)s)[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	ft_free_tab(char **tab, size_t j)
{
	while (j > 0)
		free(tab[--j]);
	free(tab);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	char	**dest;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	dest = malloc(sizeof(char *) * (ft_countwords(s, c) + 1));
	if (!dest)
		return (NULL);
	while (s[i] && j < ft_countwords(s, c))
	{
		if (!ft_is_sep(s[i], c))
		{
			dest[j] = ft_wordcpy(&s[i], c);
			if (!dest[j++])
				return (ft_free_tab(dest, j), NULL);
			while (!ft_is_sep(s[++i], c) && s[i])
				;
		}
		i++;
	}
	dest[j] = NULL;
	return (dest);
}
