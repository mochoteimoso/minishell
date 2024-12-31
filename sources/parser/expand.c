/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:58:12 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 16:20:42 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			handle_expand(t_shell *mini, t_cmd **cmd);
static int	this_is_space(t_cmd **cmd, t_expand *arg, char **expan);
static int	exp_while(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);
static int	hd(t_cmd **cmd, t_expand *arg, char **expan);
static int	quoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);

/**
 * handle_expand - Expands variables and handles heredoc in a command segment.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Double pointer to the command structure.
 *
 * Expands environment variables, handles quoted strings, and processes
 * heredoc indicators in the command's segment. Updates the segment
 * with the expanded string.
 * Returns 0 on success or 1 on failure.
 */
int	handle_expand(t_shell *mini, t_cmd **cmd)
{
	char		*expan;
	t_expand	arg;

	expan = NULL;
	if (init_expansion(&arg, &expan))
		return (1);
	if (exp_while(mini, cmd, &arg, &expan))
	{
		if (arg.value)
			free(arg.value);
		if (expan)
			free(expan);
		return (1);
	}
	free((*cmd)->seg);
	(*cmd)->seg = expan;
	return (0);
}

/**
 * exp_while - Processes the command segment for expansion and modification.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Double pointer to the command structure.
 * @arg: Pointer to the expansion context structure.
 * @expan: Pointer to the expanded string.
 *
 * Iterates over the command segment, handling spaces, quotes, heredoc,
 * and unquoted sections. Modifies the expanded string accordingly.
 * Returns 0 on success or 1 on failure.
 */
static int	exp_while(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan)
{
	while ((*cmd)->seg[arg->i])
	{
		if (arg->i != -1 && ft_isspace((*cmd)->seg[arg->i])
			&& !arg->dbl && !arg->sgl)
			arg->i = this_is_space(cmd, arg, expan);
		if (arg->i != -1 && ((*cmd)->seg[arg->i] == '\''
				|| (*cmd)->seg[arg->i] == '"'))
			arg->i = quoted(mini, cmd, arg, expan);
		if (arg->i != -1 && ((*cmd)->seg[arg->i] == '<'
				&& (*cmd)->seg[arg->i + 1] == '<'))
			arg->i = hd(cmd, arg, expan);
		else if (arg->i != -1)
			arg->i = s_unquoted(mini, cmd, arg, expan);
		if (arg->i == -1)
			return (1);
	}
	return (0);
}

/**
 * this_is_space - Appends a space character to the expanded string.
 *
 * @cmd: Double pointer to the command structure.
 * @arg: Pointer to the expansion context structure.
 * @expan: Pointer to the expanded string.
 *
 * Appends a space character from the segment to the expanded string,
 * accounting for unquoted spaces.
 * Returns the updated index or -1 on failure.
 */
static int	this_is_space(t_cmd **cmd, t_expand *arg, char **expan)
{
	char	*temp;

	(void)cmd;
	temp = *expan;
	*expan = ft_strjoin_char(temp, (*cmd)->seg[arg->i]);
	free(temp);
	if (!*expan)
		return (-1);
	arg->i++;
	return (arg->i);
}

/**
 * hd - Handles heredoc indicator in the command segment.
 *
 * @cmd: Double pointer to the command structure.
 * @arg: Pointer to the expansion context structure.
 * @expan: Pointer to the expanded string.
 *
 * Processes a heredoc indicator (<<) in the command segment and updates
 * the expanded string with the processed value.
 * Returns the updated index or -1 on failure.
 */
static int	hd(t_cmd **cmd, t_expand *arg, char **expan)
{
	char	*temp;

	temp = *expan;
	arg->i = we_have_heredoc(arg, (*cmd)->seg, 0);
	if (arg->i == -1)
		return (-1);
	*expan = ft_strjoin(temp, arg->value);
	if (!(*expan))
	{
		free(temp);
		return (-1);
	}
	free(temp);
	free(arg->value);
	arg->value = NULL;
	return (arg->i);
}

/**
 * quoted - Handles a quoted section in the command segment.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Double pointer to the command structure.
 * @arg: Pointer to the expansion context structure.
 * @expan: Pointer to the expanded string.
 *
 * Processes a quoted section, expanding its content and appending it
 * to the expanded string.
 * Returns the updated index or -1 on failure.
 */
static int	quoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan)
{
	char	*temp;

	temp = *expan;
	arg->i = in_quotes(mini, (*cmd)->seg, arg->i, arg);
	if (arg->i == -1)
		return (-1);
	*expan = ft_strjoin(temp, arg->value);
	if (!(*expan))
	{
		free(temp);
		return (-1);
	}
	free(temp);
	free(arg->value);
	arg->value = NULL;
	return (arg->i);
}
