/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 09:49:20 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/24 11:33:22 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_atoi(char const *str)
{
	int			p_n;
	long long	num;
	long long	check;

	p_n = 1;
	num = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			p_n = -1;
		str++;
	}
	while (*str && ft_isdigit(*str))
	{
		check = num * 10 + *str - 48;
		if (check / 10 != num && p_n == 1)
			return (-1);
		if (check / 10 != num && p_n == -1)
			return (0);
		num = check;
		str++;
	}
	return ((int)num * p_n);
}
