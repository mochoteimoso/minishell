/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:10:31 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:50:07 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_memmove - Copies memory safely, handling overlapping regions.
 *
 * @dest: Pointer to the destination memory.
 * @src: Pointer to the source memory.
 * @num: Number of bytes to copy.
 *
 * Returns:
 * A pointer to the destination memory.
 *
 * Details:
 * - Handles overlapping regions by copying from the end when needed.
 * - Uses `ft_memcpy` for non-overlapping memory.
 */
void	*ft_memmove(void *dest, void const *src, size_t num)
{
	unsigned char	*t_dest;
	unsigned char	*t_src;

	t_dest = (unsigned char *) dest;
	t_src = (unsigned char *) src;
	if (!dest && !src)
		return (0);
	if (t_dest > t_src)
	{
		t_dest += num - 1;
		t_src += num - 1;
		while (num > 0)
		{
			*t_dest = *t_src;
			t_dest--;
			t_src--;
			num--;
		}
	}
	else
		ft_memcpy(dest, src, num);
	return (dest);
}
