/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:09:00 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:28:10 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_isprint - Checks if a character is printable.
 *
 * @ch: The character to check.
 *
 * Returns:
 * 1 if the character is printable (space to tilde), otherwise 0.
 */
int	ft_isprint(int ch)
{
	if (ch >= ' ' && ch <= '~')
		return (1);
	else
		return (0);
}
