/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:24:50 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:49:54 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_memchr - Searches for the first occurrence of a byte in a memory block.
 *
 * @ptr: Pointer to the memory block.
 * @ch: The byte to search for.
 * @num: Number of bytes to search.
 *
 * Returns:
 * A pointer to the first occurrence of the byte, or NULL if not found.
 */
void	*ft_memchr(void const *ptr, int ch, size_t num)
{
	unsigned char	*t_ptr;
	size_t			i;

	i = 0;
	t_ptr = (unsigned char *) ptr;
	while (num > i)
	{
		if (*t_ptr == (unsigned char) ch)
			return (t_ptr);
		i++;
		t_ptr++;
	}
	return (NULL);
}
