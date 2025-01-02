/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_unquoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:17:22 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:44:26 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			s_unquoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);
int			no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg);
static int	if_check(t_cmd *cmd, t_expand *arg);
static int	this_expands(t_shell *mini, t_cmd *cmd, t_expand *arg);
static int	expand_var(t_shell *mini, char *str, char **expan, t_expand *arg);

/**
 * s_unquoted - Processes unquoted segments in the command.
 *
 * @mini: Pointer to the shell structure for context.
 * @cmd: Double pointer to the command structure.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 * @expan: Pointer to the string accumulating expanded results.
 *
 * Handles unquoted segments of a command by processing them for expansions
 * and appending results to the expanded string. Frees intermediate memory
 * as needed. Returns the updated index or -1 on failure.
 */
int	s_unquoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan)
{
	char	*temp;
	temp = *expan;
	arg->i = no_quotes(mini, *cmd, arg->i, arg);
	if (arg->i == -1)
	{
		free(temp);
		*expan = NULL;
		return (-1);
	}
	*expan = ft_strjoin(temp, arg->value);
	free(temp);
	free(arg->value);
	arg->value = NULL;
	return (arg->i);
}

/**
 * no_quotes - Processes a command segment without quotes.
 *
 * @mini: Pointer to the shell structure for context.
 * @cmd: Pointer to the command structure.
 * @i: Current index in the command segment.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 *
 * Processes the segment for variables, tildes, or other expansions when
 * no quotes are present. Returns the updated index or -1 on failure.
 */
int	no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg)
{
	the_arg(arg, i);
	if (!arg->value)
		return (-1);
	while (cmd->seg[arg->i])
	{
		if ((cmd->seg[arg->i] == '$' && (cmd->seg[arg->i + 1] == '"'
					|| cmd->seg[arg->i + 1] == '\'')) && !arg->dbl && !arg->sgl)
			arg->i++;
		if (ft_isspace(cmd->seg[arg->i])
			|| (cmd->seg[arg->i] == '\'' || cmd->seg[arg->i] == '"'))
			break ;
		if (if_check(cmd, arg) == 1)
			arg->i = this_expands(mini, cmd, arg);
		else
			arg->i = no_expanding(mini, cmd->seg, arg);
		if (arg->i == -1)
			return (-1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

/**
 * if_check - Determines if the current segment requires expansion.
 *
 * @cmd: Pointer to the command structure.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 *
 * Checks if the current segment contains a tilde ('~') or a dollar ('$')
 * that requires expansion. Returns 1 if expansion is needed, 0 otherwise.
 */
static int	if_check(t_cmd *cmd, t_expand *arg)
{
	if ((cmd->seg[arg->i] == '~' && (cmd->seg[arg->i + 1] == '/'
				|| ft_isspace(cmd->seg[arg->i + 1]) || !cmd->seg[arg->i + 1]))
		|| (cmd->seg[arg->i] == '$' && (cmd->seg[arg->i + 1]
				&& (ft_isalnum(cmd->seg[arg->i + 1])
					|| cmd->seg[arg->i + 1] == '_'
					|| cmd->seg[arg->i + 1] == '?'))))
		return (1);
	else
		return (0);
}

/**
 * this_expands - Handles expansion for the current segment.
 *
 * @mini: Pointer to the shell structure for context.
 * @cmd: Pointer to the command structure.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 *
 * Expands variables or tildes in the current segment. Returns the updated
 * index after expansion or -1 on failure.
 */
static int	this_expands(t_shell *mini, t_cmd *cmd, t_expand *arg)
{
	if ((cmd->seg[arg->i] == '~' && (cmd->seg[arg->i + 1] == '/'
				|| ft_isspace(cmd->seg[arg->i + 1]) || !cmd->seg[arg->i + 1]))
		|| (cmd->seg[arg->i] == '$' && (cmd->seg[arg->i + 1]
				&& (ft_isalnum(cmd->seg[arg->i + 1])
					|| cmd->seg[arg->i + 1] == '_'
					|| cmd->seg[arg->i + 1] == '?'))))
		arg->i = expand_var(mini, cmd->seg, &arg->value, arg);
	if (arg->i == -1)
		return (-1);
	return (arg->i);
}

/**
 * expand_var - Expands a variable or tilde in the command segment.
 *
 * @mini: Pointer to the shell structure for context.
 * @str: Input string containing the segment.
 * @expan: Pointer to the string accumulating the expanded results.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 *
 * Handles variable ('$') or tilde ('~') expansion in the segment. Updates
 * the expanded string and returns the updated index or -1 on failure.
 */
static int	expand_var(t_shell *mini, char *str, char **expan, t_expand *arg)
{
	int	cont;

	arg->start = arg->i;
	cont = arg->start;
	if (str[arg->i] == '$')
		arg->i = oh_a_dollar(mini, str, expan, arg);
	if (arg->i == -1)
		return (-1);
	else if (str[arg->i] == '~')
		arg->i = tildes_home(mini, str, expan, arg);
	if (str[cont + 1] == '?')
		arg->i = cont + 2;
	return (arg->i);
}
