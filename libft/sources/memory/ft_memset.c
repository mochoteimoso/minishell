/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:59:30 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:50:11 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_memset - Fills a memory block with a specified byte value.
 *
 * @v: Pointer to the memory block.
 * @value: Byte value to set.
 * @len: Number of bytes to set.
 *
 * Returns:
 * A pointer to the memory block.
 *
 * Details:
 * - Iterates through the memory block and sets each byte to the specified value.
 */
void	*ft_memset(void *v, int value, size_t len)
{
	unsigned char	*p;
	unsigned char	n;

	n = (unsigned char)value;
	p = v;
	while (len > 0)
	{
		*p = n;
		p++;
		len--;
	}
	return (v);
}
