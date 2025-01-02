/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:36:31 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:15:04 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_strtrim - Removes characters from the start and end of a string.
 *
 * @s1: String to trim.
 * @set: Set of characters to remove.
 *
 * Returns:
 * A pointer to the trimmed string, or NULL on memory allocation failure.
 */
char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	front;
	size_t	back;
	char	*result;

	front = 0;
	back = ft_strlen(s1) - 1;
	if (!s1 || !set)
		return (NULL);
	if (*s1 == '\0')
		return (ft_strdup(""));
	while (front <= back && ft_strchr(set, s1[front]))
		front++;
	if (front > back)
		return (ft_strdup(""));
	while (ft_strchr(set, s1[back]) && back >= 0)
		back--;
	result = (char *)malloc(sizeof(char) * (back - front + 2));
	if (!result)
		return (NULL);
	ft_strlcpy(result, &s1[front], back - front + 2);
	return (result);
}
