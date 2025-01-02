/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_countchar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:33:30 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:46:22 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_countchar - Counts the occurrences of a character in a string.
 *
 * @str: The input string.
 * @c: The character to count.
 *
 * Returns:
 * The number of times the character appears in the string.
 */
int	ft_countchar(char *str, char c)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count);
}
