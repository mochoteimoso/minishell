/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trailing_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:32:24 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:56:36 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char		*handle_trailing_pipe(char *input);
static char	*join_and_free(char *input, char *additional_input);

/**
 * handle_trailing_pipe - Handles additional input
 * 						  for commands ending with a trailing pipe.
 *
 * @input: Current input string to be appended.
 *
 * Continuously prompts the user with a secondary prompt (">")
 * to complete the command when a trailing pipe is detected.
 * Joins additional input to the current input until valid input is provided.
 * Frees memory and returns NULL if input is canceled.
 *
 * Return: The updated input string with additional input appended,
 * or NULL if canceled.
 */
char	*handle_trailing_pipe(char *input)
{
	char	*additional_input;

	additional_input = NULL;
	signal(SIGINT, sigint_handler);
	while (1)
	{
		additional_input = readline(">");
		if (!additional_input)
		{
			free(input);
			return (NULL);
		}
		if (!is_this_empty(additional_input))
			return (join_and_free(input, additional_input));
		free(additional_input);
	}
}

/**
 * join_and_free - Joins two strings and frees the original inputs.
 *
 * @input: Original input string.
 * @additional_input: Additional input string to be joined.
 *
 * Allocates memory for a new string that concatenates `input`
 * and `additional_input`.
 * Frees the memory of both input strings. If allocation fails,
 * frees remaining resources.
 *
 * Return: The newly concatenated string, or NULL on failure.
 */
static char	*join_and_free(char *input, char *additional_input)
{
	char	*updated_input;

	updated_input = ft_strjoin(input, additional_input);
	free(additional_input);
	if (!updated_input)
	{
		free(input);
		return (NULL);
	}
	free(input);
	return (updated_input);
}
