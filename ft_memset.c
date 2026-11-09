/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 12:14:50 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/09 18:59:35 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t len)
{
	unsigned char	*p;

	p = (unsigned char *)ptr;
	while (len--)
	{
		*p = (unsigned char)value;
		p++;
	}
	return (ptr);
}
