/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:34:02 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:44:10 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	just_a_quest(char *str, char *name, int *indx, t_expand *arg);
void	we_need_name(t_expand *arg, char *str, char *name, int *indx);

/**
 * just_a_quest - Handles the special case of the '?' variable in an expansion.
 *
 * @str: The input string being processed.
 * @name: Buffer to store the variable name.
 * @indx: Pointer to the index for writing into the name buffer.
 * @arg: Pointer to the expansion context structure.
 *
 * Checks if the current character in the string is '?', appends it to
 * the name buffer, and updates the buffer index and string index.
 * Ensures the name buffer is null-terminated.
 */
void	just_a_quest(char *str, char *name, int *indx, t_expand *arg)
{
	if (str[arg->i] == '?')
	{
		if (*indx < (int) sizeof(name) - 1)
			name[(*indx)++] = str[arg->i++];
	}
	name[*indx] = '\0';
}

/**
 * we_need_name - Extracts a valid variable name from the input string.
 *
 * @arg: Pointer to the expansion context structure.
 * @str: The input string being processed.
 * @name: Buffer to store the extracted variable name.
 * @indx: Pointer to the index for writing into the name buffer.
 *
 * Iterates through the input string, appending alphanumeric characters
 * or underscores to the name buffer, stopping at the first invalid character.
 * Updates the buffer and string indices accordingly, ensuring the name
 * buffer is null-terminated.
 */
void	we_need_name(t_expand *arg, char *str, char *name, int *indx)
{
	while (str[arg->i] && (ft_isalnum(str[arg->i]) || str[arg->i] == '_'))
	{
		if (*indx < 100 - 1)
			name[(*indx)++] = str[arg->i++];
	}
	name[*indx] = '\0';
}
