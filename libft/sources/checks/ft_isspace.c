/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:37:09 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:16:51 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_isspace - Checks if a character is a whitespace character.
 *
 * @c: The character to check.
 *
 * Returns:
 * 1 if the character is a space, tab, newline, carriage return,
 * or vertical tab; otherwise 0.
 */
int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' )
	{
		return (1);
	}
	else
		return (0);
}
