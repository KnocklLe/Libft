/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 11:48:06 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:49 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}
// #include <stdio.h>
// #include <ctype.h>
// int	main(void)
// {
// 	printf("test 1 'A' -> ft: %c 
// 		| sys: %c\n", ft_tolower('A'), tolower('A'));
// 	printf("test 2 '?' -> ft: %c 
// 		| sys: %c\n", ft_tolower('?'), tolower('?'));
// 	return (0);
// }