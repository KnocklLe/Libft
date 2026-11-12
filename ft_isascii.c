/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 08:12:56 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:22:12 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	else
		return (0);
}
// int	main(void)
// {
// 	#include <stdio.h>
// 	#include <ctype.h>
// 	printf("test 1 127 -> ft: %d | sys: %d\n", ft_isascii(127), isascii(127));
// 	printf("test 2 200 -> ft: %d | sys: %d\n", ft_isascii(200), isascii(200));
// 	return (0);
// }