/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:13:18 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:30:37 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_strnstr - Locates a substring in a string within a size limit.
 *
 * @haystack: String to search in.
 * @needle: Substring to find.
 * @num: Maximum number of characters to search.
 *
 * Returns:
 * A pointer to the first occurrence of `needle`, or NULL if not found.
 */
char	*ft_strnstr(char const *haystack, char const *needle, size_t num)
{
	size_t	i;
	size_t	j;

	if (needle[0] == '\0')
		return ((char *)haystack);
	if (num == 0)
		return (0);
	i = 0;
	while (haystack[i] != '\0' && i < num)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && (i + j) < num)
		{
			j++;
			if (needle[j] == '\0')
				return ((char *)(haystack + i));
		}
		i++;
	}
	return (NULL);
}
