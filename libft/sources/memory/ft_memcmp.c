/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:54:48 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:06:51 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_memcmp - Compares two memory blocks byte by byte.
 *
 * @ptr1: Pointer to the first memory block.
 * @ptr2: Pointer to the second memory block.
 * @num: Number of bytes to compare.
 *
 * Returns:
 * An integer greater than, equal to, or less than 0, depending on the comparison result.
 *
 * Details:
 * - Stops comparing when a difference is found or `num` bytes are compared.
 */
int	ft_memcmp(char *ptr1, char *ptr2, size_t num)
{
	size_t			i;
	unsigned char	*t_ptr1;
	unsigned char	*t_ptr2;

	i = 0;
	t_ptr1 = (unsigned char *) ptr1;
	t_ptr2 = (unsigned char *) ptr2;
	while (i < num)
	{
		if (t_ptr1[i] != t_ptr2[i])
			return (t_ptr1[i] - t_ptr2[i]);
		i++;
	}
	return (0);
}
