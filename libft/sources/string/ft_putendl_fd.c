/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:41:53 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:50:24 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_putendl_fd - Outputs a string followed by a newline
 * 				   to the given file descriptor.
 *
 * @s: String to output.
 * @fd: File descriptor to write to.
 *
 * Details:
 * - Writes the string followed by a newline character.
 * - Does nothing if the string is NULL.
 */
void	ft_putendl_fd(char *s, int fd)
{
	if (s)
	{
		write(fd, s, ft_strlen(s));
		write(fd, "\n", 1);
	}
}
