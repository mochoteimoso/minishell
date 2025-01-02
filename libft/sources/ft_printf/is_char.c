/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_char.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 10:17:27 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:39:24 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

/**
 * is_char - Prints a single character.
 *
 * @c: The character to print.
 * @check: Pointer to the error flag.
 *
 * Returns:
 * 1 if successful, or -1 on error.
 */
int	is_char(int c, int *check)
{
	if (*check == -1)
		return (-1);
	if (write(1, &c, 1) == -1)
	{
		*check = -1;
		return (-1);
	}
	return (1);
}
