/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:26:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/08/07 15:12:37 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdarg.h>

int	check_form(char const *str, va_list args, int *check);
int	form_id(char const *str, va_list args, int *check);
int	ft_printf(char const *str, ...);
int	is_char(int c, int *check);
int	is_hex(unsigned long lng, int cap, int *check);
int	is_nbr(int n, int *check);
int	is_pointer(void *ptr, int *check);
int	is_str(char *s, int *check);
int	is_unsignbr(unsigned int n, int *check);

#endif
