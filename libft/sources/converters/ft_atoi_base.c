/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 14:27:39 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:29:56 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lower - Converts an uppercase character to lowercase.
 *
 * @c: The character to convert.
 *
 * Returns:
 * The lowercase equivalent if the input is uppercase,
 * otherwise the original character.
 */
static char	ft_lower(char c)
{
	if (c >= 65 && c <= 90)
		return (c + (97 - 65));
	return (c);
}

/**
 * put_nbr - Converts a character to its integer representation
 * 			 in a specific base.
 *
 * @c: The character to convert.
 * @base: The numeric base (e.g., 10 for decimal, 16 for hexadecimal).
 *
 * Returns:
 * The integer value of the character in the given base, or -1 if invalid.
 */
static int	put_nbr(char c, int base)
{
	int	max;

	if (base <= 10)
		max = base + 48;
	else
		max = base - 10 + 97;
	if (c >= 48 && c <= 57 && c <= max)
		return (c - 48);
	else if (c >= 97 && c <= 102 && c <= max)
		return (10 + c - 97);
	else
		return (-1);
}

/**
 * ft_atoi_base - Converts a string representation of a number
 * 				  in a specific base to an integer.
 *
 * @str: The string containing the number.
 * @base: The numeric base of the number.
 *
 * Returns:
 * The integer representation of the number, or -1 if conversion fails.
 */
int	ft_atoi_base(const char *str, int base)
{
	int	res;
	int	p_n;
	int	num;

	res = 0;
	p_n = 1;
	num = 0;
	if (!str)
		return (-1);
	if (*str == '-')
	{
		p_n = -1;
		str++;
	}
	while (num >= 0)
	{
		num = put_nbr(ft_lower(*str), base);
		if (num < 0)
			break ;
		res = res * base;
		res = res + num;
		str++;
	}
	return (res * p_n);
}
