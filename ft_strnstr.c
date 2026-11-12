/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 11:09:45 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:27:15 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!little[0])
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] && little[j]
			&& (i + j) < len && big[i + j] == little[j])
			j++;
		if (!little[j])
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
// #include <stdio.h>
// int	main(void)
// {
// 	printf("test 1 (World, len 11) -> ft: %s | attendu: World\n", 
// 		ft_strnstr("Hello World", "World", 11));
// 	printf("test 2 (World, len 8)  -> ft: %p | attendu: (nil)\n", 
// 		ft_strnstr("Hello World", "World", 8));
// 	return (0);
// }