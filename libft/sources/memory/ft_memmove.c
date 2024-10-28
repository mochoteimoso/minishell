/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:10:31 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/24 11:34:28 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

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
