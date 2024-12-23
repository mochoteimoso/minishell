/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:58:12 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/23 14:59:17 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_expand(t_shell *mini, t_cmd **cmd);
static int	this_is_space(t_cmd **cmd, t_expand *arg, char **expan);
static int	expand_while(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);
static int	seg_no_quotes(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);
static int	hd_quoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);

int	handle_expand(t_shell *mini, t_cmd **cmd)
{
	char		*expan;
	t_expand	arg;

	expan = NULL;
	if (init_expansion(&arg, &expan))
		return (1);
	if (expand_while(mini, cmd, &arg, &expan))
		return (1);
	free((*cmd)->seg);
	(*cmd)->seg = expan;
	return (0);
}

static int	expand_while(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan)
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
			arg->i = seg_no_quotes(mini, cmd, arg, expan);
			if (arg->i == -1)
				return (1);
		}
	}
	return (0);
}

static int	this_is_space(t_cmd **cmd, t_expand *arg, char **expan)
{
	char	*temp;

	temp = ft_strdup(*expan);
	if (!temp)
		return (-1);
	free(*expan);
	// temp = *expan;
	*expan = ft_strjoin_char(temp, (*cmd)->seg[arg->i]);
	if (!*expan)
		return (-1);
	free(temp);
	arg->i++;
	return (arg->i);
}

static int	hd_quoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan)
{
	char	*temp;

	if ((*cmd)->seg[arg->i] == '<' && (*cmd)->seg[arg->i + 1] == '<')
	{
		temp = ft_strdup(*expan);
		free(*expan);
		// temp = *expan;
		arg->i = we_have_heredoc(arg, (*cmd)->seg);
		*expan = ft_strjoin(temp, arg->value);
		free(temp);
	}
	if ((*cmd)->seg[arg->i] == '\'' || (*cmd)->seg[arg->i] == '"')
	{
		temp = ft_strdup(*expan);
		free(*expan);
		// temp = *expan;
		arg->i = in_quotes(mini, (*cmd)->seg, arg->i, arg);
		*expan = ft_strjoin(temp, arg->value);
		free(temp);
	}
	// free(arg->value);
	return (arg->i);
}

static int	seg_no_quotes(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan)
{
	char	*temp;

	temp = ft_strdup(*expan);
	free(*expan);
	// temp = *expan;
	arg->i = no_quotes(mini, *cmd, arg->i, arg);
	// printf("temp: {%s}\nvalue: {%s}\n", temp, arg->value);
	*expan = ft_strjoin(temp, arg->value);
	// printf("exapan: {%s}\n", *expan);
	free(temp);
	free(arg->value);
	return (arg->i);
}
