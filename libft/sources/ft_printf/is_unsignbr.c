/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_unsignbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:53:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/25 14:18:07 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	is_unsignbr(unsigned int n, int *check)
{
	int	c;

	c = 0;
	if (n >= 10)
	{
		c += is_nbr(n / 10, check);
		c += is_char(n % 10 + 48, check);
	}
	else
		c += is_char(n % 10 + 48, check);
	return (c);
}
