/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 10:07:27 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:47:27 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_toupper - Converts a lowercase letter to uppercase.
 *
 * @c: The character to convert.
 *
 * Returns:
 * The uppercase equivalent if the input is lowercase,
 * otherwise the original character.
 */
int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	else
		return (c);
}
