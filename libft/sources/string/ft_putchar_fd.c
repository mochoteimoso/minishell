/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 17:29:24 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:50:20 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_putchar_fd - Outputs a character to the given file descriptor.
 *
 * @c: Character to output.
 * @fd: File descriptor to write to.
 *
 * Details:
 * - Uses the `write` system call to output a single character.
 */
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
