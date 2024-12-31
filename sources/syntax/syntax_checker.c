/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:45:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 17:39:39 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_input_syntax(char **input, t_shell *mini);
int	check_quotes(char *input, int limit);
int	is_this_empty(char *input);

/**
 * validate_input_syntax - Validates the syntax of the input command string.
 *
 * @input: Pointer to the input string.
 * @mini: Shell structure for managing shell state and exit status.
 *
 * Checks the input for unmatched quotes, invalid pipe usage,
 * and illegal characters such as `;` and `\\`.
 * Sets the shell's exit status to 2 for syntax errors.
 *
 * Return: 1 if syntax is invalid, 0 otherwise.
 */
int	validate_input_syntax(char **input, t_shell *mini)
{
	int	i;

	if (check_quotes(*input, ft_strlen(*input)))
	{
		ft_putendl_fd("syntax error: unmatched quotes", 2);
		mini->exit_stat = 2;
		return (1);
	}
	if (check_pipes(input, mini))
		return (1);
	i = 0;
	while ((*input)[i])
	{
		if (!check_quotes(*input, i) && ((*input)[i] == ';'
			|| (*input)[i] == '\\'))
		{
			ft_putendl_fd("invalid syntax", 2);
			mini->exit_stat = 2;
			return (1);
		}
		i++;
	}
	if (check_redirects(*input, mini))
		return (1);
	return (0);
}

/**
 * check_quotes - Verifies if quotes in the input are properly matched.
 *
 * @input: Input string to check.
 * @limit: Maximum index to check in the string (-1 to check the entire string).
 *
 * Tracks the state of single and double quotes
 * and ensures that they are properly closed by the end of the input.
 * If `limit` is specified, checks up to the given index.
 *
 * Return: 1 if unmatched quotes are found, 0 otherwise.
 */
int	check_quotes(char *input, int limit)
{
	int	in_single_quote;
	int	in_double_quote;
	int	index;

	in_single_quote = 0;
	in_double_quote = 0;
	index = 0;
	while (input[index] && (limit == -1 || index < limit))
	{
		if (input[index] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[index] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		index++;
	}
	if (limit == -1)
	{
		return (in_single_quote || in_double_quote);
	}
	return (in_single_quote || in_double_quote);
}

/**
 * is_this_empty - Checks if the input string contains only whitespace.
 *
 * @input: Input string to check.
 *
 * Iterates through the input
 * and ensures it contains no non-whitespace characters.
 *
 * Return: 1 if the input is empty or only whitespace, 0 otherwise.
 */
int	is_this_empty(char *input)
{
	while (*input)
	{
		if (!ft_isspace(*input))
			return (0);
		input++;
	}
	return (1);
}
