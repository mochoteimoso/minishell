/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_name.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:35:20 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 17:18:59 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			handle_cmd_name(t_cmd *cmd, int i);
static int	quoted_cmd(t_expand *name, char *seg);
static int	this_is_redirection(t_cmd *cmd, t_expand *name);

/**
 * handle_cmd_name - Parses and extracts the command name
 * 					 from the command segment.
 *
 * @cmd: Pointer to the command structure.
 * @i: Index in the command segment to start processing.
 *
 * Skips leading whitespace, then identifies
 * and processes the command name. Handles
 * quoted command names and stops parsing when encountering a redirection or pipe
 * symbol. Allocates memory for and assigns the command name to `cmd->command`.
 * Returns the updated index on success, or 1 on error.
 */
int	handle_cmd_name(t_cmd *cmd, int i)
{
	t_expand	name;

	i = skip_whitespace(cmd->seg, i);
	the_arg(&name, i);
	while (cmd->seg[name.i])
	{
		if (this_is_redirection(cmd, &name))
			break ;
		if (cmd->seg[name.i] == '\'' || cmd->seg[name.i] == '"')
		{
			name.i = quoted_cmd(&name, cmd->seg);
			if (ft_strlen(name.value) == 0)
			{
				free(name.value);
				cmd->command = ft_strdup("''");
				return (name.i);
			}
			break ;
		}
		else if (add_char(cmd->seg, &name))
			return (free(cmd->seg), 1);
	}
	cmd->command = ft_strdup(name.value);
	free(name.value);
	return (name.i);
}

/**
 * this_is_redirection - Checks if the current character is
 * 						 a redirection or pipe symbol.
 *
 * @cmd: Pointer to the command structure.
 * @name: Pointer to the t_expand structure,
 * 		  which contains the current index and state.
 *
 * Returns 1 if the current character is a redirection
 * (`<` or `>`) or pipe (`|`), or if it is whitespace. Otherwise, returns 0.
 */
static int	this_is_redirection(t_cmd *cmd, t_expand *name)
{
	if (cmd->seg[name->i] == ' ' || cmd->seg[name->i] == '<'
		|| cmd->seg[name->i] == '>' || cmd->seg[name->i] == '|')
		return (1);
	else
		return (0);
}

/**
 * quoted_cmd - Parses a command name enclosed in quotes.
 *
 * @name: Pointer to the t_expand structure,
 * 		  which tracks the current parsing state.
 * @seg: The command segment being processed.
 *
 * Processes single-quoted or double-quoted command names, updating the quote
 * state as needed. Stops parsing at unquoted spaces or end of the quotes.
 * Appends the characters to `name->value`.
 * Returns the updated index in the segment on success,
 * or -1 on memory allocation error.
 */
static int	quoted_cmd(t_expand *name, char *seg)
{
	what_quote(seg, name);
	while (seg[name->i])
	{
		if (seg[name->i] == ' ' && !name->sgl && !name->dbl)
			break ;
		else if (!name->sgl && !name->dbl && (seg[name->i] == '\''
				|| seg[name->i] == '"'))
		{
			what_quote(seg, name);
		}
		else if ((name->sgl && seg[name->i] == '\'')
			|| (name->dbl && seg[name->i] == '"'))
		{
			what_quote(seg, name);
		}
		else if (add_char(seg, name))
			return (free(name->value), -1);
	}
	name->len = ft_strlen(name->value);
	return (name->i);
}
