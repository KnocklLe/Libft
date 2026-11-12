/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 08:14:45 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:21:40 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0'
			&& c <= '9'))
		return (1);
	else
		return (0);
}
/*
int	main(void)
{
	#include <stdio.h>
	#include <ctype.h>
	printf("test 1 'Z' -> ft: %d | sys: %d\n", ft_isalnum('Z'), isalnum('Z'));
	printf("test 2 '@' -> ft: %d | sys: %d\n", ft_isalnum('@'), isalnum('@'));
	return (0);
}*/
