/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 09:57:32 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:50:35 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_putstr_fd - Outputs a string to the given file descriptor.
 *
 * @s: String to output.
 * @fd: File descriptor to write to.
 *
 * Details:
 * - Writes the string to the file descriptor using `write`.
 * - Does nothing if the string is NULL.
 */
void	ft_putstr_fd(char *s, int fd)
{
	if (s != NULL)
	{
		write(fd, s, ft_strlen(s));
	}
}
