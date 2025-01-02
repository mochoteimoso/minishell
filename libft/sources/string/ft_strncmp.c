/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:58:37 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:14:32 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_strncmp - Compares up to `num` characters of two strings.
 *
 * @s1: First string.
 * @s2: Second string.
 * @num: Number of characters to compare.
 *
 * Returns:
 * 0 if the strings are equal, a positive value if s1 > s2, or a negative value if s1 < s2.
 */
int	ft_strncmp(char const *s1, char const *s2, size_t num)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && (s1[i] == s2[i]) && i < (num - 1))
		i++;
	if ((num == 0) || (s1[i] == s2[i]))
		return (0);
	else
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
