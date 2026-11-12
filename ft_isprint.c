/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 08:16:02 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:22:25 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	else
		return (0);
}
//  #include <stdio.h>
//  #include <ctype.h>

// int	main(void)
// {
//  	printf("test 1 ' '  -> ft: %d 
// | sys: %d\n", ft_isprint(' '), isprint(' '));
// 	printf("test 2 '\\n' -> ft: %d 
// | sys: %d\n", ft_isprint('\n'), isprint('\n'));
// 	return (0);
// }
