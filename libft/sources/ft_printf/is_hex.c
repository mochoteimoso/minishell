/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_hex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:39:44 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/25 14:17:51 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	is_hex(unsigned long lng, int cap, int *check)
{
	int	c;

	c = 0;
	if (lng >= 16)
	{
		c += is_hex((lng / 16), cap, check);
		c += is_hex((lng % 16), cap, check);
		if (c == -1)
			return (-1);
	}
	else if ((cap == 0) && (lng >= 10 && lng <= 15))
		c += is_char((lng + 87), check);
	else if ((cap == 1) && (lng >= 10 && lng <= 15))
		c += is_char((lng + 55), check);
	else if (lng < 10)
		c += is_nbr(lng, check);
	return (c);
}
