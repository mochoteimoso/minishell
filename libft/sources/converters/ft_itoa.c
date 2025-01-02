/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:03:34 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:47:18 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * number_len - Calculates the number of digits in an integer.
 *
 * @nb: The input number.
 *
 * Returns:
 * The length of the number in digits, including the sign if negative.
 */
static int	number_len(long nb)
{
	int	count;

	count = 0;
	if (nb < 0)
	{
		nb = nb * -1;
		count++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

/**
 * ft_itoa - Converts an integer to its string representation.
 *
 * @n: The integer to convert.
 *
 * Returns:
 * A dynamically allocated string representing the integer.
 */
char	*ft_itoa(int n)
{
	char	*str;
	long	num;
	int		i;

	num = n;
	i = number_len(num);
	if (num == 0)
		return (ft_strdup("0"));
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (str == 0)
		return (0);
	str[i] = '\0';
	i--;
	if (num < 0)
	{
		str[0] = '-';
		num = num * -1;
	}
	while (num > 0)
	{
		str[i] = 48 + (num % 10);
		num = num / 10;
		i--;
	}
	return (str);
}
