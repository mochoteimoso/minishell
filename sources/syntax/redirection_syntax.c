/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:06:48 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:56:28 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			check_redirects(char *input, t_shell *mini);
static int	check_in_redir(char *input, t_shell *mini, int *i);
static int	check_out_redir(char *input, t_shell *mini, int *i);
static int	validate_redirect(char *input, t_shell *mini, int *i, char *type);

/**
 * check_redirects - Validates the syntax of redirection operators
 * 					 in the input string.
 *
 * @input: Input string to check.
 * @mini: Shell structure for managing shell state and exit status.
 *
 * Scans the input for redirection operators (`>`, `>>`, `<`, `<<`)
 * outside of quotes and ensures they are followed by a valid token
 * (not `|`, `<`, `>`, or EOF).
 *
 * Return: 1 if redirection syntax is invalid, 0 otherwise.
 */
int	check_redirects(char *input, t_shell *mini)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' && !check_quotes(input, i))
		{
			if (check_in_redir(input, mini, &i))
				return (1);
		}
		else if (input[i] == '<' && !check_quotes(input, i))
		{
			if (check_out_redir(input, mini, &i))
				return (1);
		}
		i++;
	}
	return (0);
}

/**
 * check_in_redir - Validates input redirection syntax.
 *
 * @input: Input string to check.
 * @mini: Shell structure for managing shell state and exit status.
 * @i: Pointer to the current index in the input string.
 *
 * Handles `>` and `>>` syntax. Advances the index appropriately
 * and validates the token following the redirection operator.
 *
 * Return: 1 if input redirection syntax is invalid, 0 otherwise.
 */
static int	check_in_redir(char *input, t_shell *mini, int *i)
{
	if (input[*i + 1] == '>')
	{
		(*i)++;
		if (validate_redirect(input, mini, i, ">>") != 0)
			return (1);
	}
	else
	{
		if (validate_redirect(input, mini, i, ">") != 0)
			return (1);
	}
	return (0);
}

/**
 * check_out_redir - Validates output redirection syntax.
 *
 * @input: Input string to check.
 * @mini: Shell structure for managing shell state and exit status.
 * @i: Pointer to the current index in the input string.
 *
 * Handles `<` and `<<` syntax. Advances the index appropriately
 * and validates the token following the redirection operator.
 *
 * Return: 1 if output redirection syntax is invalid, 0 otherwise.
 */
static int	check_out_redir(char *input, t_shell *mini, int *i)
{
	if (input[*i + 1] == '<')
	{
		(*i)++;
		if (validate_redirect(input, mini, i, "<<") != 0)
			return (1);
	}
	else
	{
		if (validate_redirect(input, mini, i, "<") != 0)
			return (1);
	}
	return (0);
}

/**
 * validate_redirect - Ensures that a redirection operator is
 * 					   followed by a valid token.
 *
 * @input: Input string to check.
 * @mini: Shell structure for managing shell state and exit status.
 * @i: Pointer to the current index in the input string.
 * @type: Redirection type (e.g., `>`, `>>`, `<`, `<<`) for error messaging.
 *
 * Skips whitespace after the redirection operator and verifies the next token is
 * valid (not `|`, `<`, `>`, or EOF). Prints an error message if invalid.
 *
 * Return: 1 if the syntax is invalid, 0 otherwise.
 */
static int	validate_redirect(char *input, t_shell *mini, int *i, char *type)
{
	(*i)++;
	while (input[*i] == ' ')
		(*i)++;
	if (!input[*i] || input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		ft_putstr_fd("syntax error near unexpected token ", 2);
		ft_putendl_fd(type, 2);
		mini->exit_stat = 2;
		return (1);
	}
	return (0);
}
