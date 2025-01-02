/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 11:00:50 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:39:09 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

/**
 * ft_printf - A custom implementation of printf for formatted output.
 *
 * @str: The format string containing specifiers and plain text.
 * @...: Variable arguments to format and print based on specifiers.
 *
 * Returns:
 * The number of characters printed, or -1 on error.
 */
int	ft_printf(char const *str, ...)
{
	int		num_c;
	int		check;
	va_list	args;

	num_c = 0;
	check = 0;
	va_start(args, str);
	num_c = check_form(str, args, &check);
	if (check == -1)
	{
		va_end(args);
		return (-1);
	}
	va_end(args);
	return (num_c);
}
