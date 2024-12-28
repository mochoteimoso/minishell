/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_unquoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:17:22 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/28 18:08:43 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg);
static int	if_check(t_cmd *cmd, t_expand *arg);
static int	this_expands(t_shell *mini, t_cmd *cmd, t_expand *arg);
static int	expand_var(t_shell *mini, char *str, char **expan, t_expand *arg);

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
