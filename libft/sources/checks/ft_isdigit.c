/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 09:25:48 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:46:46 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_isdigit - Checks if a character is a numeric digit.
 *
 * @ch: The character to check.
 *
 * Returns:
 * 1 if the character is a digit ('0' to '9'), otherwise 0.
 */
int	ft_isdigit(int ch)
{
	if (ch >= '0' && ch <= '9')
		return (1);
	else
		return (0);
}
