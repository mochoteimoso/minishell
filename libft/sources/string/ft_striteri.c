/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:56:59 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 13:35:25 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_striteri - Applies a function to each character of a string with its index.
 *
 * @s: String to iterate over.
 * @f: Function to apply.
 *
 * Details:
 * - Applies the function `f` to each character of the string `s`.
 * - Each call to `f` is passed the current index and a pointer to the character.
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t	i;

	i = 0;
	if (!s || !f)
		return ;
	if (s && f)
	{
		while (*s)
			f(i++, s++);
	}
}
