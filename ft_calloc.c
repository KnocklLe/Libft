/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 13:56:54 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:22:04 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	size_t	totalsize;
	char	*dest;

	totalsize = nmemb * size;
	dest = malloc(sizeof(char) * totalsize);
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (i < totalsize)
	{
		dest[i] = 0;
		i++;
	}
	return (dest);
}
