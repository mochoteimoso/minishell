/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:26:32 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:31:14 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_strrchr - Finds the last occurrence of a character in a string.
 *
 * @s: String to search.
 * @c: Character to locate.
 *
 * Returns:
 * A pointer to the last occurrence of the character, or NULL if not found.
 */
char	*ft_strrchr(char const *s, int c)
{
	char	*ptr_s;
	char	ch;
	int		i;

	ptr_s = (char *)s;
	ch = (char)c;
	i = 0;
	while (ptr_s[i] != '\0')
		i++;
	while (i >= 0)
	{
		if (ptr_s[i] == ch)
			return (&ptr_s[i]);
		i--;
	}
	return (NULL);
}
