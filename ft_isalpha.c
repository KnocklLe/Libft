/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 07:59:38 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:21:53 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}
/*
int	main(void)
{
	#include <stdio.h>
	#include <ctype.h>
	printf("test 1 'a' -> ft: %d | sys: %d\n", ft_isalpha('a'), isalpha('a'));
	printf("test 2 '5' -> ft: %d | sys: %d\n", ft_isalpha('5'), isalpha('5'));
	return (0);
}*/