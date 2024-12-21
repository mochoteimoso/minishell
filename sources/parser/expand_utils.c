/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:12:11 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/21 14:21:40 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	we_have_heredoc(t_expand *arg, char *str)
{
	arg->value = ft_strdup("");
	while (str[arg->i] == '<' || str[arg->i] == ' ')
		add_char(str, arg);
	while (str[arg->i] && (!ft_isspace(str[arg->i]) || !(str[arg->i] == '<') || !(str[arg->i] == '>')))
		add_char(str, arg);
	return (arg->i);
}

static int	no_expanding(t_shell *mini, char *str, t_expand *arg, int i)
{
	arg->sgl = 0;
	arg->dbl = 0;
	arg->i = i;
	arg->start = i;
	what_quote(str, arg);
	while (str[arg->i])
	{
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
			i = we_have_dollar(mini, arg, str);
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

int	segment_no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg)
{
	the_arg(arg, i);
	if (!arg->value)
		return (-1);
	while (cmd->segment[i])
	{
		if ((cmd->segment[i] == '$' && (cmd->segment[i + 1] == '"' || cmd->segment[i + 1] == '\'')) && !arg->dbl && !arg->sgl)
			i++;
		if (ft_isspace(cmd->segment[i]) || (cmd->segment[i] == '\'' || cmd->segment[i] == '"'))
			break ;
		if ((cmd->segment[i] == '~' && (cmd->segment[i + 1] == '/' || ft_isspace(cmd->segment[i + 1]) || !cmd->segment[i + 1]))
			|| (cmd->segment[i] == '$' && (cmd->segment[i + 1] && (ft_isalnum(cmd->segment[i + 1])
			|| cmd->segment[i + 1] == '_' || cmd->segment[i + 1] == '?'))))
		{
			arg->i = i;
			i = expand_variable(mini, cmd->segment, &arg->value, arg);
		}
		else
		{
			arg->i = i;
			i = no_expanding(mini, cmd->segment, arg, i);
		}
	}
	arg->len = ft_strlen(arg->value);
	return (i);
}

int	segment_in_quotes(t_shell *mini, char *str, int i, t_expand *arg)
{
	char	*temp;

	if (the_arg(arg, i))
		return (-1);
	temp = arg->value;
	arg->value = ft_strjoin_char(temp, str[arg->i]);
	free(temp);
	what_quote(str, arg);
	while (str[arg->i])
	{
		if (!arg->sgl && !arg->dbl)
			break ;
		if (str[arg->i] == '$' && str[arg->i + 1] && (str[arg->i + 1] == '\'' || str[arg->i + 1] == '"') && (!arg->sgl && !arg->dbl))
			arg->i++;
		if ((str[arg->i] == '$' && !arg->sgl) && (str[arg->i + 1] && !ft_isspace(str[arg->i + 1])
			&& (ft_isalnum(str[arg->i + 1]) || str[arg->i + 1] == '_' || str[arg->i + 1] == '?')))
		{
			if (we_have_dollar(mini, arg, str) == -1)
				return (free(arg->value), -1);
		}
		else if (!arg->sgl && !arg->dbl && (str[arg->i] == '\'' || str[arg->i] == '"'))
			what_quote(str, arg);
		else if ((arg->sgl && str[arg->i] == '\'') || (arg->dbl && str[arg->i] == '"'))
		{
			temp = arg->value;
			arg->value = ft_strjoin_char(temp, str[arg->i]);
			what_quote(str, arg);
			free(temp);
		}
		else if (add_char(str, arg))
			return (free(arg->value), -1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}
