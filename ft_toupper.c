/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 11:00:58 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:51 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}
// #include <stdio.h>
// #include <ctype.h>
// int	main(void)
// {
// 	printf("test 1 'a' -> ft: %c 
// 		| sys: %c\n", ft_toupper('a'), toupper('a'));
// 	printf("test 2 '5' -> ft: %c 
// 		| sys: %c\n", ft_toupper('5'), toupper('5'));
// 	return (0);
// }