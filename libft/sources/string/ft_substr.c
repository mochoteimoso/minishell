/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:46:31 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:51:38 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_substr - Extracts a substring from a string.
 *
 * @s: Original string.
 * @start: Start index of the substring.
 * @len: Maximum length of the substring.
 *
 * Returns:
 * A pointer to the substring, or NULL on memory allocation failure.
 */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret;
	size_t	i;
	size_t	l_sub;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	l_sub = ft_strlen(s) - start;
	if (len < l_sub)
		l_sub = len;
	ret = (char *)malloc(sizeof(char) * (l_sub + 1));
	if (!ret)
		return (NULL);
	while (i < len && s[start] != '\0')
	{
		ret[i] = s[start];
		start++;
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
