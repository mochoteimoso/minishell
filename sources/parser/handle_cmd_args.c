/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:26:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/23 12:15:23 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_cmd_args(t_shell *mini, t_cmd *cmd, int i);
int	count_args(t_cmd *cmd, int i);

int	this_is_quote(t_cmd *cmd, t_expand *arg)
{
	char	*temp;

	if (!arg->sgl && !arg->dbl && (cmd->seg[arg->i] == '\'' || cmd->seg[arg->i] == '"'))
	{
		temp = arg->value;
		arg->value = ft_strjoin_char(temp, cmd->seg[arg->i]);
		what_quote(cmd->seg, arg);
		free(temp);
	}
	else if ((arg->sgl && cmd->seg[arg->i] == '\'')
		|| (arg->dbl && cmd->seg[arg->i] == '"'))
	{
		temp = arg->value;
		arg->value = ft_strjoin_char(arg->value, cmd->seg[arg->i]);
		what_quote(cmd->seg, arg);
		free(temp);
	}
	return (arg->i);
}

int export_args(t_cmd *cmd, t_expand *arg, int i)
{
	the_arg(arg, i);
	what_quote(cmd->seg, arg);
	while (cmd->seg[arg->i])
	{
		if (ft_isspace(cmd->seg[arg->i]) && !arg->sgl && !arg->dbl)
			break ;
		if (cmd->seg[arg->i] == '\'' || cmd->seg[arg->i] == '"')
			arg->i = this_is_quote(cmd, arg);
		else if (add_char(cmd->seg, arg))
			return (free(arg->value), -1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

int	handle_export_redir(t_cmd *cmd, t_expand *arg, int i, int *arg_index)
{
	i = export_args(cmd, arg, i);
	if (!arg->value || append_to_array(cmd, arg->value, arg_index) == -1)
	{
		free(arg->value);
		ft_free_array(cmd->args);
		return (-1);
	}
	free(arg->value);
	i = skip_whitespace(cmd->seg, i);
	return (i);
}

int	handle_arg(t_cmd *cmd, int i, t_expand *arg, int *arg_index)
{
	if (ft_strcmp(cmd->command, "export") == 0 && only_redirect(cmd->seg, i))
	{
		i = handle_export_redir(cmd, arg, i, arg_index);
		if (i == -1)
			return (-1);
		return (i);
	}
	if (cmd->seg[i] == '\'' || cmd->seg[i] == '"')
		i = arg_in_quotes(cmd->seg, i, arg);
	else if (!ft_isspace(cmd->seg[i]) || !is_redirection(cmd, i))
		i = arg_no_quotes(cmd, arg, i);
	if (i == -1)
		return (-1);
	if (!arg->value || append_to_array(cmd, arg->value, arg_index) == -1)
	{
		free(arg->value);
		ft_free_array(cmd->args);
		return (-1);
	}
	free(arg->value);
	i = skip_whitespace(cmd->seg, i);
	return (i);
}

int	handle_cmd_args(t_shell *mini, t_cmd *cmd, int i)
{
	int			arg_index;
	t_expand	arg;

	if (init_args_array(cmd, i) == -1)
		return (-1);
	arg_index = 1;
	i = skip_whitespace(cmd->seg, i);
	the_arg(&arg, i);
	free(arg.value);
	while (cmd->seg[i] && arg_index < cmd->args_count + 1)
	{
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(mini, cmd, i);
			if (i == -1)
				return (-1);
			i = skip_whitespace(cmd->seg, i);
			continue;
		}
		i = handle_arg(cmd, i, &arg, &arg_index);
		if (i == -1)
			return (-1);
	}
	cmd->args[arg_index] = NULL;
	return (i);
}

