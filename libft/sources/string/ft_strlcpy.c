/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:00:29 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:14:06 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_strlcpy - Copies a string with a size limit.
 *
 * @dest: Destination buffer.
 * @src: Source string to copy.
 * @size: Size of the destination buffer.
 *
 * Returns:
 * The length of the source string.
 */
size_t	ft_strlcpy(char *dest, char const *src, size_t size)
{
	size_t	s;
	size_t	len_src;

	s = 0;
	len_src = ft_strlen(src);
	if (size == 0)
		return (len_src);
	while (s < size - 1 && src[s])
	{
		dest[s] = src[s];
		s++;
	}
	dest[s] = '\0';
	return (len_src);
}
