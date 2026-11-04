/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 13:56:54 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/04 14:30:26 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
    size_t     i;
    size_t  totalsize;
    char    *dest;

    totalsize = nmemb * size;
    dest = malloc(sizeof(char) * totalsize);
    if (dest == NULL)
        return(NULL);
    i = 0;
    while (i < totalsize)
    {
        dest[i] = 0;
        i++;
    }
    return (dest);
}
/*
int	main(void)
{
    char    *dest = ft_calloc(2, 4);
    char    *dest2 = calloc(2, 4);
    if (dest != NULL)
    {
        dest[0] = 'C';
        dest[1] = 'a';
        dest[2] = 's';
        dest[3] = 'e';
        
        printf("Mon texte : %s\n", dest);
    }
    free(dest);
    if (dest2 != NULL)
    {
        dest2[0] = 'C';
        dest2[1] = 'a';
        dest2[2] = 's';
        dest2[3] = 'e';
        
        printf("Mon texte : %s\n", dest2);
    }
    free(dest2);
}*/