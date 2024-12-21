/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:12:21 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/21 19:48:03 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	skip_whitespace(char *str, int i)
{
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

int	arg_no_quotes(t_cmd *cmd, t_expand *arg, int i)
{
	the_arg(arg, i);
	what_quote(cmd->seg, arg);
	while (cmd->seg[arg->i])
	{
		if ((cmd->seg[arg->i] == ' ' || cmd->seg[arg->i] == '\t' || cmd->seg[arg->i] == '>' || cmd->seg[arg->i] == '<') && !arg->sgl && !arg->dbl)
			break ;
		if (!arg->sgl && !arg->dbl && (cmd->seg[arg->i] == '\'' || cmd->seg[arg->i] == '"'))
			what_quote(cmd->seg, arg);
		else if ((arg->sgl && cmd->seg[arg->i] == '\'')
			|| (arg->dbl && cmd->seg[arg->i] == '"'))
			what_quote(cmd->seg, arg);
		else if (add_char(cmd->seg, arg))
			return (free(arg->value), -1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

int	arg_in_quotes(char *str, int i, t_expand *arg)
{
	if (the_arg(arg, i))
		return (-1);
	what_quote(str, arg);
	while (str[arg->i])
	{
		if (str[arg->i] == '$' && str[arg->i + 1] && (str[arg->i + 1] == '\'' || str[arg->i + 1] == '"') && (!arg->sgl && !arg->dbl))
			arg->i++;
		else if (!arg->sgl && !arg->dbl && (str[arg->i] == '\'' || str[arg->i] == '"'))
			what_quote(str, arg);
		else if ((arg->sgl && str[arg->i] == '\'') || (arg->dbl && str[arg->i] == '"'))
			what_quote(str, arg);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
		if (str[arg->i] == ' ' && !arg->sgl && !arg->dbl)
			break ;
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

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
