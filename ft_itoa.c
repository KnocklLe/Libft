/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 11:15:25 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:22:31 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_size(int n)
{
	int	temp;
	int	count;

	temp = n;
	count = 0;
	while (temp != 0)
	{
		temp = temp / 10;
		count++;
	}
	if (n <= 0)
		count++;
	return (count);
}

char	*ft_itoa(int n)
{
	int		count;
	char	*dest;
	int		i;
	long	nb;

	count = ft_size(n);
	nb = n;
	dest = malloc(sizeof(char) * (count + 1));
	if (dest == NULL)
		return (NULL);
	if (nb == 0)
		dest[0] = '0';
	if (nb < 0)
		dest[0] = '-';
	i = count - 1;
	if (nb > 0)
		nb = -nb;
	while (nb != 0)
	{
		dest[i] = -(nb % 10) + '0';
		nb = nb / 10;
		i--;
	}
	dest[count] = '\0';
	return (dest);
}
// #include <stdio.h>
// #include <stdlib.h>
// int	main(void)
// {
// 	char	*r;

// 	r = ft_itoa(-42);
// 	printf("test 1 -42         -> ft: %s | attendu: -42\n", r);
// 	free(r);
// 	r = ft_itoa(-2147483648);
// 	printf("test 2 -2147483648 -> ft: %s | attendu: -2147483648\n", r);
// 	free(r);
// 	return (0);
// }