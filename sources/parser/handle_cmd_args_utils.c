/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:12:21 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/03 17:26:23 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	skip_whitespace(char *str, int i);
int	arg_no_quotes(t_cmd *cmd, t_expand *arg, int i);
int	arg_in_quotes(char *str, int i, t_expand *arg);
int	append_to_array(t_cmd *cmd, char *arg, int *index);

/**
 * skip_whitespace - Skips over whitespace in a string.
 *
 * @str: The input string.
 * @i: The starting index.
 *
 * Advances the index to skip over any leading whitespace characters.
 * Returns the updated index.
 */
int	skip_whitespace(char *str, int i)
{
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

/**
 * arg_no_quotes - Extracts an argument without quotes from a command segment.
 *
 * @cmd: Pointer to the command structure.
 * @arg: Pointer to the argument expansion structure.
 * @i: The starting index in the command segment.
 *
 * Parses and handles arguments that are not enclosed in quotes. Accounts for
 * spaces, redirection symbols, and expands arguments as necessary.
 * Returns the updated index or -1 on failure.
 */
int	arg_no_quotes(t_cmd *cmd, t_expand *arg, int i)
{
	the_arg(arg, i);
	what_quote(cmd->seg, arg);
	while (cmd->seg[arg->i])
	{
		if ((ft_isspace(cmd->seg[arg->i]) || cmd->seg[arg->i] == '>'
				|| cmd->seg[arg->i] == '<') && !arg->sgl && !arg->dbl)
			break ;
		if (!arg->sgl && !arg->dbl
			&& (cmd->seg[arg->i] == '\'' || cmd->seg[arg->i] == '"'))
			what_quote(cmd->seg, arg);
		else if ((arg->sgl && cmd->seg[arg->i] == '\'')
			|| (arg->dbl && cmd->seg[arg->i] == '"'))
			what_quote(cmd->seg, arg);
		else if (add_char(cmd->seg, arg))
			return (free(arg->value), -1);
	}
	return (arg->i);
}

/**
 * arg_in_quotes - Extracts an argument enclosed in quotes from a string.
 *
 * @str: The input string.
 * @i: The starting index in the string.
 * @arg: Pointer to the argument expansion structure.
 *
 * Handles arguments enclosed in single or double quotes, including nested quotes
 * and special characters. Updates the expansion structure
 * with the extracted argument.
 * Returns the updated index or -1 on failure.
 */
int	arg_in_quotes(char *str, int i, t_expand *arg)
{
	if (the_arg(arg, i))
		return (-1);
	what_quote(str, arg);
	while (str[arg->i])
	{
		if (str[arg->i] == '$' && str[arg->i + 1]
			&& (str[arg->i + 1] == '\'' || str[arg->i + 1] == '"')
			&& (!arg->sgl && !arg->dbl))
			arg->i++;
		else if (!arg->sgl && !arg->dbl
			&& (str[arg->i] == '\'' || str[arg->i] == '"'))
			what_quote(str, arg);
		else if ((arg->sgl && str[arg->i] == '\'')
			|| (arg->dbl && str[arg->i] == '"'))
			what_quote(str, arg);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
		if (ft_isspace(str[arg->i]) && !arg->sgl && !arg->dbl)
			break ;
	}
	return (arg->i);
}

/**
 * append_to_array - Appends an argument to the command's argument array.
 *
 * @cmd: Pointer to the command structure.
 * @arg: The argument string to append.
 * @index: Pointer to the current index in the argument array.
 *
 * Allocates memory for the new argument and adds it to the array. Increments the
 * argument index. Returns 0 on success or -1 on failure.
 */
int	append_to_array(t_cmd *cmd, char *arg, int *index)
{
	cmd->args[*index] = ft_strdup(arg);
	if (!cmd->args[*index])
	{
		ft_putendl_fd("Failed to allocate memory for argument", 2);
		return (-1);
	}
	(*index)++;
	return (0);
}
