/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_str.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 10:39:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/25 14:18:04 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	s_len(char *l)
{
	int	n;

	n = 0;
	while (*l)
	{
		l++;
		n++;
	}
	return (n);
}

int	is_str(char *s, int *check)
{
	int	len;

	if (!s)
		return (write(1, "(null)", 6));
	len = s_len(s);
	if (write(1, s, len) == -1)
	{
		*check = -1;
		return (-1);
	}
	return (len);
}
