/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_str.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 10:39:55 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:48:36 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

/**
 * s_len - Calculates the length of a string.
 *
 * @l: The string to measure.
 *
 * Returns:
 * The length of the string.
 */
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

/**
 * is_str - Prints a string.
 *
 * @s: The string to print.
 * @check: Pointer to the error flag.
 *
 * Returns:
 * The number of characters printed, or -1 on error.
 */
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
