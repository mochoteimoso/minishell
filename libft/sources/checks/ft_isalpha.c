/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:11:24 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:27:32 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_isalpha - Checks if a character is alphabetic.
 *
 * @ch: The character to check.
 *
 * Returns:
 * 1 if the character is a letter (uppercase or lowercase), otherwise 0.
 */
int	ft_isalpha(int ch)
{
	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		return (1);
	else
		return (0);
}
