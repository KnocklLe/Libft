/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edolemai <edolemai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 12:05:11 by edolemai          #+#    #+#             */
/*   Updated: 2026/11/03 12:44:29 by edolemai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}
/*int main(void)
{
	#include <stdio.h>
	char *s1 = "Helloa";
	char *s2 = "Helloz World!";
	int n = 5;

	int result = ft_strncmp(s1, s2, n);
	printf("Result: %d\n", result);

	return (0);
}*/