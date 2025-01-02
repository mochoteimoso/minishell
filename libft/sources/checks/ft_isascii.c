/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:05:34 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:46:35 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_isascii - Checks if a character is an ASCII character.
 *
 * @ch: The character to check.
 *
 * Returns:
 * 1 if the character is within the ASCII range (0 to 127), otherwise 0.
 */
int	ft_isascii(int ch)
{
	if (ch >= 0 && ch <= 127)
		return (1);
	else
		return (0);
}
