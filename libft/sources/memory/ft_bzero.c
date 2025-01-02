/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:29:28 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:06:11 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_bzero - Sets a block of memory to zero.
 *
 * @ptr: Pointer to the block of memory to be zeroed.
 * @num: Number of bytes to set to zero.
 *
 * Details:
 * - Iterates through the memory block and sets each byte to 0.
 */
void	ft_bzero(void *ptr, size_t num)
{
	unsigned char	*temp;

	temp = (unsigned char *) ptr;
	while (num > 0)
	{
		*temp = 0;
		temp++;
		num--;
	}
}
