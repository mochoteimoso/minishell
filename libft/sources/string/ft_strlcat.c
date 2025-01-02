/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:55:34 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:13:57 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_strlcat - Concatenates strings with a size limit.
 *
 * @dest: Destination string.
 * @src: Source string to append.
 * @size: Total size of the destination buffer.
 *
 * Returns:
 * The length of the string it tried to create.
 */
size_t	ft_strlcat(char *dest, char const *src, size_t size)
{
	size_t	i;
	size_t	l_dst;
	size_t	s_total;
	size_t	s_remain;

	i = 0;
	l_dst = ft_strlen(dest);
	s_total = l_dst + ft_strlen(src);
	if (size == 0 || size <= l_dst)
		return (ft_strlen(src) + size);
	else
		s_remain = size - l_dst - 1;
	while (src[i] && s_remain > i)
	{
		dest[l_dst + i] = src[i];
		i++;
	}
	dest[l_dst + i] = '\0';
	return (s_total);
}
