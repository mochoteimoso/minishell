/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 12:52:44 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:10:51 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_strcmp - Compares two strings lexicographically.
 *
 * @s1: First string.
 * @s2: Second string.
 *
 * Returns:
 * An integer greater than, equal to, or less than 0, depending on the comparison result.
 *
 * Details:
 * - Iterates through both strings, comparing characters until a difference is found or the end is reached.
 */
int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}
