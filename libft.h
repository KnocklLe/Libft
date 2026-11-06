/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 14:57:31 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/06 13:23:21 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include <strings.h>
# include <unistd.h>

void	*ft_bzero(void *tab, size_t n);
int		ft_strlen(const char *str);
char	*ft_strdup(const char *s);
int		ft_strlcpy(char *dest, const char *src, int size);
void	ft_putendl_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);

#endif