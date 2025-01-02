/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:49:16 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:06:21 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"
#include <stdint.h>

/**
 * ft_calloc - Allocates and initializes memory for an array.
 *
 * @num_elem: Number of elements to allocate.
 * @elem_size: Size of each element.
 *
 * Returns:
 * A pointer to the allocated memory initialized to zero, or NULL if allocation fails.
 *
 * Details:
 * - Checks for overflow in `num_elem * elem_size`.
 * - Allocates memory and initializes it to zero using `ft_bzero`.
 */
void	*ft_calloc(size_t num_elem, size_t elem_size)
{
	void	*mem;

	if (num_elem != 0)
	{
		if (elem_size > SIZE_MAX / num_elem)
			return (NULL);
	}
	mem = malloc(num_elem * elem_size);
	if (!mem)
		return (NULL);
	ft_bzero(mem, num_elem * elem_size);
	return (mem);
}
