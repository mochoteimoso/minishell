/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:47:04 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:07:01 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_memcpy - Copies memory from source to destination.
 *
 * @dest: Pointer to the destination memory.
 * @src: Pointer to the source memory.
 * @n: Number of bytes to copy.
 *
 * Returns:
 * A pointer to the destination memory.
 *
 * Details:
 * - Does not handle overlapping memory blocks.
 * - Returns NULL if both `dest` and `src` are NULL.
 */
void	*ft_memcpy(void *dest, void const *src, size_t n)
{
	unsigned char	*tdest;
	unsigned char	*tsrc;

	tdest = (unsigned char *) dest;
	tsrc = (unsigned char *) src;
	if (dest == NULL && src == NULL)
		return (NULL);
	while (n > 0)
	{
		*tdest = *tsrc;
		tdest++;
		tsrc++;
		n--;
	}
	return (dest);
}
