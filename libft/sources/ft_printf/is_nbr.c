/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_nbr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:10:29 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:39:49 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

/**
 * is_nbr - Converts and prints an integer.
 *
 * @n: The integer to print.
 * @check: Pointer to the error flag.
 *
 * Returns:
 * The number of characters printed, or -1 on error.
 */
int	is_nbr(int n, int *check)
{
	int	i;

	i = 0;
	if (n == -2147483648)
		i += write(1, "-2147483648", 11);
	else if (n < 0)
	{
		i += is_char('-', check);
		i += is_nbr(-n, check);
	}
	else if (n >= 10)
	{
		i += is_nbr(n / 10, check);
		i += is_char(n % 10 + 48, check);
	}
	else
		i += is_char(n % 10 + 48, check);
	return (i);
}
