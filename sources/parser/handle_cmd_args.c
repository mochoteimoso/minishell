/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:26:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/03 13:58:45 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_cmd_args(t_shell *mini, t_cmd *cmd, int i);
int	count_args(t_cmd *cmd, int i);

int	count_if_redirection(t_cmd *cmd, int i)
{
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) &&
		is_redirection(cmd, i))
		i++;
	return (i);
}

// Counts how many command arguments the string contains

int	count_args(t_cmd *cmd, int i)
{
	int	args_count;

	args_count = 0;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	while (cmd->segment[i])
	{
		if (is_redirection(cmd, i))
		{
			i++;
			i = count_if_redirection(cmd, i);
		}
		else
		{
			args_count++;
			while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) &&
			!is_redirection(cmd, i))
				i++;
		}
		while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
			i++;
	}
	return (args_count);
}

/*int	handle_cmd_args(t_shell *mini, t_cmd *cmd, int i)
{
	int			args_count;
	int			arg_index;
	t_expand	arg;

	arg_index = 0;
	args_count = count_args(cmd, i);
	cmd->args = ft_calloc(args_count + 2, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[arg_index] = ft_strdup(cmd->command);
	if (!cmd->args[0])
	{
		ft_free_array(cmd->args);
		return (-1);
	}
	arg_index++;
	i = skip_whitespace(cmd->segment, i);
	while (cmd->segment[i] && arg_index < args_count + 1 && !is_redirection(cmd, i))
	{
		if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
			i = arg_in_quotes(mini, cmd->segment, i, &arg);
		else
			i = arg_no_quotes(mini, cmd, i, &arg);
		if (i == -1)
			return (-1);
		arg.len = ft_strlen(arg.value);
		if (!arg.value || append_to_array(cmd, arg.value, arg.len, &arg_index) == -1)
		{
			free(arg.value);
			ft_free_array(cmd->args);
			return (-1);
		}
		free(arg.value);
		i = skip_whitespace(cmd->segment, i);
	}
	cmd->args[arg_index] = NULL;
	return (i);
} */

int	handle_cmd_args(t_shell *mini, t_cmd *cmd, int i)
{
	int			args_count;
	int			arg_index;
	t_expand	arg;

	arg_index = 0;
	args_count = count_args(cmd, i);
	cmd->args = ft_calloc(args_count + 2, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[arg_index] = ft_strdup(cmd->command);
	if (!cmd->args[0])
	{
		ft_free_array(cmd->args);
		return (-1);
	}
	arg_index++;
	i = skip_whitespace(cmd->segment, i);
	while (cmd->segment[i] && arg_index < args_count + 1)
	{
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(cmd, i);
			if (i == -1)
				return (-1);
			i = skip_whitespace(cmd->segment, i);
			continue;
		}
		if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
			i = arg_in_quotes(mini, cmd->segment, i, &arg);
		else
			i = arg_no_quotes(mini, cmd, i, &arg);
		if (i == -1)
			return (-1);
		arg.len = ft_strlen(arg.value);
		if (!arg.value || append_to_array(cmd, arg.value, arg.len, &arg_index) == -1)
		{
			free(arg.value);
			ft_free_array(cmd->args);
			return (-1);
		}
		free(arg.value);
		i = skip_whitespace(cmd->segment, i);
	}
	cmd->args[arg_index] = NULL;
	return (i);
}
