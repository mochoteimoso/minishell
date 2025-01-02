/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:18:09 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:54:41 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			count_args(t_cmd *cmd, int i);
int			count_if_redirection(t_cmd *cmd, int i);
int			only_redirect(char *str, int i);
int			init_args_array(t_cmd *cmd, int i);
static int	skip_quoted_segment(const char *seg, int i);

/**
 * skip_quoted_segment - Skips over a quoted segment in the command.
 *
 * @seg: The command segment string.
 * @i: The starting index of the quote.
 *
 * Advances the index to skip over the quoted section, accounting for both
 * single and double quotes. Returns the updated index.
 */
static int	skip_quoted_segment(const char *seg, int i)
{
	char	quote;

	quote = seg[i];
	i++;
	while (seg[i] && seg[i] != quote)
		i++;
	if (seg[i] == quote)
		i++;
	return (i);
}

/**
 * count_args - Counts the number of arguments in a command segment.
 *
 * @cmd: Pointer to the command structure.
 * @i: The starting index in the command segment.
 *
 * Iterates through the command segment to count arguments,
 * handling redirections,
 * quoted arguments, and whitespace. Returns the total argument count.
 */
int	count_args(t_cmd *cmd, int i)
{
	int	args_count;

	args_count = 0;
	while (cmd->seg[i] && ft_isspace(cmd->seg[i]))
		i++;
	while (cmd->seg[i])
	{
		if (is_redirection(cmd, i))
			i = count_if_redirection(cmd, ++i);
		else
		{
			args_count++;
			while (cmd->seg[i] && (!ft_isspace(cmd->seg[i])
					|| check_quotes(cmd->seg, i)) && !is_redirection(cmd, i))
			{
				if (cmd->seg[i] == '\'' || cmd->seg[i] == '"')
					i = skip_quoted_segment(cmd->seg, i);
				else
					i++;
			}
		}
		while (cmd->seg[i] && ft_isspace(cmd->seg[i]))
			i++;
	}
	return (args_count);
}

/**
 * count_if_redirection - Skips over redirection operators
 * 						  in the command segment.
 *
 * @cmd: Pointer to the command structure.
 * @i: Current index in the command segment.
 *
 * Advances the index past redirection operators and any associated whitespace.
 * Returns the updated index.
 */
int	count_if_redirection(t_cmd *cmd, int i)
{
	while (cmd->seg[i] && ft_isspace(cmd->seg[i]))
		i++;
	while (cmd->seg[i] && !ft_isspace(cmd->seg[i])
		&& is_redirection(cmd, i))
		i++;
	return (i);
}

/**
 * only_redirect - Checks if a string contains only
 * 				   a redirection operator after '='.
 *
 * @str: The input string.
 * @i: The starting index in the string.
 *
 * Looks for an '=' followed by a redirection operator ('>', '<', '|').
 * Returns 1 if a redirection operator is found, otherwise 0.
 */
int	only_redirect(char *str, int i)
{
	int	n;

	n = i;
	while (str[n] && str[n] != '=')
		n++;
	if (!str[n])
		return (0);
	n++;
	if (str[n])
		n++;
	if ((str[n] == '>' || str[n] == '<') || str[n] == '|')
		return (1);
	return (0);
}

/**
 * init_args_array - Initializes the arguments array for a command.
 *
 * @cmd: Pointer to the command structure.
 * @i: Starting index for parsing arguments in the command segment.
 *
 * Allocates memory for the command's arguments array based on the number
 * of arguments. Sets the first argument to the command name.
 * Returns 0 on success or -1 on memory allocation failure.
 */
int	init_args_array(t_cmd *cmd, int i)
{
	cmd->a_num = count_args(cmd, i) + 1;
	cmd->args = ft_calloc(cmd->a_num + 1, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[0] = ft_strdup(cmd->command);
	if (!cmd->args[0])
	{
		ft_free_array(cmd->args);
		return (-1);
	}
	return (0);
}
