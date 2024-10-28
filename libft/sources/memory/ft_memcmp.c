/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:54:48 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/24 11:34:28 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

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
