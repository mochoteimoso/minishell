/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:06:20 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/03 16:53:26 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			check_pipes(char **input, t_shell *mini);
static int	check_consecutive_pipes(char *input, t_shell *mini);
static int	check_pipe_error(char *input, int i, t_shell *mini);
static int	check_trailing_pipe(t_shell *mini, char **input);

/**
 * check_pipes - Validates the syntax of pipes (`|`) in the input string.
 *
 * @input: Pointer to the input string, may be updated for trailing pipes.
 * @mini: Shell structure for managing shell state and exit status.
 *
 * Ensures the input doesn't start with a pipe, doesn't contain consecutive pipes
 * without valid tokens between them, and properly handles trailing pipes.
 *
 * Return: 1 if pipe syntax is invalid, 0 otherwise.
 */
int	check_pipes(char **input, t_shell *mini)
{
	int	i;

	i = 0;
	i = skip_whitespace(*input, i);
	if ((*input)[i] == '|' && !check_quotes(*input, i))
	{
		ft_putendl_fd("syntax error near unexpected token ", 2);
		mini->exit_stat = 2;
		return (1);
	}
	if (check_consecutive_pipes(*input, mini))
		return (1);
	if (check_trailing_pipe(mini, input))
		return (1);
	return (0);
}

/**
 * check_consecutive_pipes - Validates that consecutive pipes
 * 							 are properly handled.
 *
 * @input: Input string to check.
 * @mini: Shell structure for managing shell state and exit status.
 *
 * Scans the input string for consecutive pipes without valid tokens
 * between them, skipping quoted characters and whitespace.
 *
 * Return: 1 if syntax errors are found, 0 otherwise.
 */
static int	check_consecutive_pipes(char *input, t_shell *mini)
{
	int	i;
	int	pipe_found;

	i = 0;
	pipe_found = 0;
	while (input[i])
	{
		if (input[i] == '|' && !check_quotes(input, i))
		{
			if (check_pipe_error(input, i, mini))
				return (1);
			pipe_found = 1;
		}
		else if (ft_isspace(input[i]))
			pipe_found = 0;
		i++;
	}
	return (0);
}

/**
 * check_pipe_error - Validates syntax for a specific pipe operator.
 *
 * @input: Input string to check.
 * @i: Current index of the pipe operator.
 * @mini: Shell structure for managing shell state and exit status.
 *
 * Skips whitespace after the pipe operator and ensures the next token is valid
 * (not another pipe or EOF). Prints an error message if invalid.
 *
 * Return: 1 if the syntax is invalid, 0 otherwise.
 */
static int	check_pipe_error(char *input, int i, t_shell *mini)
{
	int	j;

	j = i + 1;
	j = skip_whitespace(input, j);
	if (input[j] == '|' && !check_quotes(input, j))
	{
		ft_putstr_fd("syntax error near unexpected token ", 2);
		ft_putendl_fd(&input[i], 2);
		mini->exit_stat = 2;
		return (1);
	}
	return (0);
}

/**
 * check_trailing_pipe - Handles trailing pipes in the input string.
 *
 * @input: Pointer to the input string, may be updated to include more input.
 *
 * If the input ends with a pipe, prompts the user for additional input via
 * `handle_trailing_pipe` and updates the input string. Returns an error if
 * additional input is not provided.
 *
 * Return: 1 if handling fails, 0 otherwise.
 */
static int	check_trailing_pipe(t_shell *mini, char **input)
{
	int		i;
	char	*updated_input;

	i = ft_strlen(*input) - 1;
	while (i >= 0 && ft_isspace((*input)[i]))
		i--;
	if (i >= 0 && (*input)[i] == '|' && !check_quotes(*input, i))
	{
		updated_input = handle_trailing_pipe(mini, *input);
		if (!updated_input)
		{
			*input = NULL;
			return (1);
		}
		*input = updated_input;
	}
	return (0);
}
