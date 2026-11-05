/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 11:15:25 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/05 11:35:38 by edolemai         ###   ########.fr       */
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
/*int	main(void)
{
#include <stdio.h>

	printf("Result : [%s]\n", ft_itoa(10));
}*/