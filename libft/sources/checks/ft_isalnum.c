/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 09:58:32 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:16:42 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_isalnum - Checks if a character is alphanumeric.
 *
 * @ch: The character to check.
 *
 * Returns:
 * 1 if the character is a digit or a letter (uppercase or lowercase),
 * otherwise 0.
 */
int	ft_isalnum(int ch)
{
	if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || \
		(ch >= 'a' && ch <= 'z'))
		return (1);
	else
		return (0);
}
