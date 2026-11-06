/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 11:28:10 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:29:19 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	main(void)
{
	char	str[5];
	int		n;

	*str = "Hello";
	n = 5;
	printf("Before bzero: %s\n", str);
	ft_bzero(str, n);
	printf("After bzero: %s\n", str);
	return (0);
}
