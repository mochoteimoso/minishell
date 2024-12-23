/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:17:22 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/23 14:32:55 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg);
static int	this_expands(t_shell *mini, t_cmd *cmd, t_expand *arg);
static int	expand_var(t_shell *mini, char *str, char **expan, t_expand *arg);
static int	no_expanding(t_shell *mini, char *str, t_expand *arg);

int	no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg)
{
	the_arg(arg, i);
	if (!arg->value)
		return (-1);
	while (cmd->seg[arg->i])
	{
		// printf("value: {%s}\ni: %d\n", arg->value, arg->i);
		if ((cmd->seg[arg->i] == '$' && (cmd->seg[arg->i + 1] == '"'
					|| cmd->seg[arg->i + 1] == '\'')) && !arg->dbl && !arg->sgl)
			arg->i++;
		if (ft_isspace(cmd->seg[arg->i])
			|| (cmd->seg[arg->i] == '\'' || cmd->seg[arg->i] == '"'))
			break ;
		if ((cmd->seg[arg->i] == '~' && (cmd->seg[arg->i + 1] == '/'
				|| ft_isspace(cmd->seg[arg->i + 1]) || !cmd->seg[arg->i + 1]))
		|| (cmd->seg[arg->i] == '$' && (cmd->seg[arg->i + 1]
				&& (ft_isalnum(cmd->seg[arg->i + 1]) || cmd->seg[arg->i + 1] == '_'
					|| cmd->seg[arg->i + 1] == '?'))))
			arg->i = this_expands(mini, cmd, arg);
		else
			arg->i = no_expanding(mini, cmd->seg, arg);
		// printf("no_quotes arg->value: {%s}\narg->i: %d\n", arg->value, arg->i);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

static int	this_expands(t_shell *mini, t_cmd *cmd, t_expand *arg)
{
	if ((cmd->seg[arg->i] == '~' && (cmd->seg[arg->i + 1] == '/'
				|| ft_isspace(cmd->seg[arg->i + 1]) || !cmd->seg[arg->i + 1]))
		|| (cmd->seg[arg->i] == '$' && (cmd->seg[arg->i + 1]
				&& (ft_isalnum(cmd->seg[arg->i + 1]) || cmd->seg[arg->i + 1] == '_'
					|| cmd->seg[arg->i + 1] == '?'))))
		arg->i = expand_var(mini, cmd->seg, &arg->value, arg);
	return (arg->i);
}

static int	expand_var(t_shell *mini, char *str, char **expan, t_expand *arg)
{
	int	cont;

	arg->start = arg->i;
	cont = arg->start;
	if (str[arg->i] == '$')
		arg->i = oh_its_a_dollar(mini, str, expan, arg);
	else if (str[arg->i] == '~')
		arg->i = tildes_home(mini, str, expan, arg);
	if (str[cont + 1] == '?')
		arg->i = cont + 2;
	// printf("arg->value: {%s}\ni: %d\n", arg->value, arg->i);
	return (arg->i);
}

static int	no_expanding(t_shell *mini, char *str, t_expand *arg)
{;
	// the_arg(arg, arg->i);
	arg->sgl = 0;
	arg->dbl = 0;
	arg->start = arg->i;
	what_quote(str, arg);
	while (str[arg->i])
	{
		// printf("no_exp value: {%s}\ni: %d\n", arg->value, arg->i);
		if (((str[arg->i] == ' ' || str[arg->i] == '\t') || (str[arg->i] == '\'' || str[arg->i] == '"')) || arg->dbl || arg->sgl)
			break ;
		if (str[arg->i] == '$' && (str[arg->i + 1] == '"' || str[arg->i + 1] == '\'') && !arg->dbl && !arg->sgl)
			arg->i++;
		else if (!arg->sgl && !arg->dbl && (str[arg->i] == '\'' || str[arg->i] == '"'))
		{
			arg->value = ft_strjoin_char(arg->value, str[arg->i]);
			what_quote(str, arg);
		}
		else if ((arg->sgl && str[arg->i] == '\'') || (arg->dbl && str[arg->i] == '"'))
		{
			arg->value = ft_strjoin_char(arg->value, str[arg->i]);
			what_quote(str, arg);
		}
		if ((((arg->dbl && !arg->sgl) || (!arg->dbl && !arg->sgl)) && str[arg->i] == '$' && ((str[arg->i + 1] && ft_isalnum(str[arg->i + 1])) || str[arg->i + 1] == '?')))
			we_have_dollar(mini, arg, str);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
		if (str[arg->i] == '<' && str[arg->i + 1] == '<')
		{
			arg->i = we_have_heredoc(arg, str);
			if (str[arg->i] == '\0')
				break ;
		}
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}
