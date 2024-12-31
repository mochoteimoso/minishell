/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:58:12 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 11:14:58 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			handle_expand(t_shell *mini, t_cmd **cmd);
static int	this_is_space(t_cmd **cmd, t_expand *arg, char **expan);
static int	exp_while(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);
static int	s_unquoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);
static int	hd_quoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);

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

static int	exp_while(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan)
{
	while ((*cmd)->seg[arg->i])
	{
		if (ft_isspace((*cmd)->seg[arg->i]) && !arg->dbl && !arg->sgl)
		{
			arg->i = this_is_space(cmd, arg, expan);
			if (arg->i == -1)
				return (1);
		}
		if ((*cmd)->seg[arg->i] == '\'' || (*cmd)->seg[arg->i] == '"'
			|| ((*cmd)->seg[arg->i] == '<' && (*cmd)->seg[arg->i + 1] == '<'))
		{
			arg->i = hd_quoted(mini, cmd, arg, expan);
			if (arg->i == -1)
				return (1);
		}
		else
		{
			arg->i = s_unquoted(mini, cmd, arg, expan);
			if (arg->i == -1)
				return (1);
		}
	}
	return (0);
}

static int	this_is_space(t_cmd **cmd, t_expand *arg, char **expan)
{
	char	*temp;

	temp = *expan;
	*expan = ft_strjoin_char(temp, (*cmd)->seg[arg->i]);
	free(temp);
	if (!*expan)
		return (-1);
	arg->i++;
	return (arg->i);
}

static int	hd_quoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan)
{
	char	*temp;

	if ((*cmd)->seg[arg->i] == '<' && (*cmd)->seg[arg->i + 1] == '<')
	{
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
	}
	if ((*cmd)->seg[arg->i] == '\'' || (*cmd)->seg[arg->i] == '"')
	{
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
	}
	return (arg->i);
}

static int	s_unquoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan)
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
