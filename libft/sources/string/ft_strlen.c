/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:37:51 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:14:13 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_strlen - Calculates the length of a string.
 *
 * @str: String to measure.
 *
 * Returns:
 * The number of characters in the string, excluding the null terminator.
 */
size_t	ft_strlen(char const *str)
{
	size_t	length;

	length = 0;
	while (*str != '\0')
	{
		str++;
		length++;
	}
	return (length);
}
