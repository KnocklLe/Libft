/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 13:38:03 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:22:50 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	if (src == NULL || n <= 0)
		return (NULL);
	while (n-- > 0)
		*((unsigned char *)dest++) = *((const unsigned char *)src++);
	*((unsigned char *)dest) = '\0';
	return (dest);
}
