/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 08:06:36 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:22:19 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}
// #include <stdio.h>
// #include <ctype.h>
// int	main(void)
// {
// 	printf("test 1 '7' -> ft: %d | sys: %d\n", ft_isdigit('7'), isdigit('7'));
// 	printf("test 2 'x' -> ft: %d | sys: %d\n", ft_isdigit('x'), isdigit('x'));
// 	return (0);
// }