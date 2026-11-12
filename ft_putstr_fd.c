/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 08:27:10 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 11:23:09 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
		write(fd, &s[i++], 1);
}
// int	main(void)
// {
// 	ft_putstr_fd("test 1 Hello 42\n", 1);
// 	ft_putstr_fd("test 2 ", 1);
// 	ft_putstr_fd("", 1);
// 	ft_putstr_fd("(rien avant ce texte)\n", 1);
// 	return (0);
// }