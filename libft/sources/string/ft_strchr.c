/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 10:37:57 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:50:43 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_strchr - Locates the first occurrence of a character in a string.
 *
 * @s: String to search.
 * @c: Character to locate.
 *
 * Returns:
 * A pointer to the first occurrence of the character, or NULL if not found.
 */
char	*ft_strchr(char const *s, int c)
{
	while (*s != '\0' && *s != (char)c)
		s++;
	if (*s == (char)c)
		return ((char *) s);
	return (NULL);
}
