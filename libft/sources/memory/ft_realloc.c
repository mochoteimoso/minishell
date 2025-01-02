/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:25:17 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:50:15 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_realloc - Resizes an allocated memory block.
 *
 * @ptr: Pointer to the existing memory block.
 * @old: Size of the existing memory block.
 * @new: New size of the memory block.
 *
 * Returns:
 * A pointer to the resized memory block, or NULL if allocation fails.
 *
 * Details:
 * - Allocates new memory of size `new`.
 * - Copies data from the old memory block up to the smaller of `old` or `new`.
 * - Frees the old memory block.
 */
void	*ft_realloc(void *ptr, size_t old, size_t new)
{
	void	*new_ptr;
	size_t	copy;

	new_ptr = malloc(new);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	if (old < new)
		copy = old;
	else
		copy = new;
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, copy);
		free(ptr);
	}
	return (new_ptr);
}
