/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_f.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:35:05 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/24 11:35:32 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	form_id(char const *str, va_list args, int *check)
{
	int	i;

	i = 0;
	if (*str == 'c')
		i += is_char(va_arg(args, int), check);
	else if (*str == 's')
		i += is_str(va_arg(args, char *), check);
	else if (*str == 'd' || *str == 'i')
		i += is_nbr(va_arg(args, int), check);
	else if (*str == '%')
		i += is_char('%', check);
	else if (*str == 'X')
		i += is_hex(va_arg(args, unsigned int), 1, check);
	else if (*str == 'x')
		i += is_hex(va_arg(args, unsigned int), 0, check);
	else if (*str == 'u')
		i += is_unsignbr(va_arg(args, unsigned int), check);
	else if (*str == 'p')
		i += is_pointer(va_arg(args, void *), check);
	if (*check == -1)
		return (-1);
	return (i);
}

int	check_form(char const *str, va_list args, int *check)
{
	int	n;

	n = 0;
	while (*str != '\0')
	{
		if (*str != '%')
		{
			n += is_char(*str, check);
			if (*check == -1)
				return (-1);
		}
		else if (*str == '%')
		{
			str++;
			n += form_id(str, args, check);
			if (*check == -1)
				return (-1);
		}
		if (!*str)
			return (0);
		str++;
	}
	return (n);
}
