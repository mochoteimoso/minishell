/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:04:07 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:49:50 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_free_array - Frees a NULL-terminated array of strings.
 *
 * @array: Pointer to the array of strings to free.
 *
 * Details:
 * - Iterates through the array, freeing each string.
 * - Frees the array itself.
 */
void	ft_free_array(char **array)
{
	int	a;

	a = 0;
	if (!array)
		return ;
	while (array[a] != NULL)
	{
		free(array[a]);
		a++;
	}
	free(array);
}
