/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_len.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:59:02 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:24:39 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_array_len - Calculates the number of strings in a null-terminated array.
 *
 * @str: Null-terminated array of strings.
 *
 * Returns:
 * The number of strings in the array.
 */
int	ft_array_len(char **str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}
