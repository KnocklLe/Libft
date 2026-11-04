/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 14:45:30 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/04 15:33:54 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


char    *ft_substr(char const *s, unsigned int start, size_t len)
{
    char    *dest;
    size_t  i;
    size_t  s_len;

    if (!s)
        return (NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
        return (ft_strdup(""));
    if (len > s_len - start)
        len = s_len - start;
    dest = malloc(sizeof(char) * (len + 1));
    if (!dest)
        return (NULL);
    i = 0;
    while (i < len)
        dest[i++] = s[start++];
    dest[i] = '\0';
    return (dest);
}
/*int	main(void)
{
    #include <stdio.h>

    printf("%s\n", ft_substr("je test la fonction", 3, 10));
}*/