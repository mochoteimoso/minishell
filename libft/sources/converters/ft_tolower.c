/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 10:33:10 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:34:13 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_tolower - Converts an uppercase letter to lowercase.
 *
 * @c: The character to convert.
 *
 * Returns:
 * The lowercase equivalent if the input is uppercase,
 * otherwise the original character.
 */
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	else
		return (c);
}
