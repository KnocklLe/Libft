/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 15:39:50 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:20:15 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_size;
	char	*finalstring;
	int		i;
	int		j;

	j = 0;
	i = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	total_size = ft_strlen(s1) + ft_strlen(s2);
	finalstring = malloc(sizeof(char) * (total_size + 1));
	if (finalstring == NULL)
		return (NULL);
	while (s1[i])
	{
		finalstring[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		finalstring[i + j] = s2[j];
		j++;
	}
	finalstring[i + j] = '\0';
	return (finalstring);
}
