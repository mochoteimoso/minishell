/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:26:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/20 14:00:58 by henbuska         ###   ########.fr       */
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

int	init_args_array(t_cmd *cmd, int i)
{
	cmd->args = ft_calloc(cmd->args_count = count_args(cmd, i) + 2, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[0] = ft_strdup(cmd->command);
	if (!cmd->args[0])
	{
		ft_free_array(cmd->args);
		return (-1);
	}
	return (0);
}

int export_args(t_shell *mini, t_cmd *cmd, t_expand *arg, int i)
{
		arg->sgl = 0;
	arg->dbl = 0;
	arg->i = i;
	arg->start = i;
	(void)mini;
	what_quote(cmd->segment, arg);
	while (cmd->segment[arg->i])
	{
		// printf("arg->value: {%s}\nlen: %d\n", arg->value, (int)ft_strlen(arg->value));
		if (ft_isspace(cmd->segment[arg->i]) && !arg->sgl && !arg->dbl)
			break ;
		if (!arg->sgl && !arg->dbl && (cmd->segment[arg->i] == '\'' || cmd->segment[arg->i] == '"'))
		{
			arg->value = ft_strjoin_char(arg->value, cmd->segment[arg->i]);
			what_quote(cmd->segment, arg);
		}
		else if ((arg->sgl && cmd->segment[arg->i] == '\'')
			|| (arg->dbl && cmd->segment[arg->i] == '"'))
		{

			arg->value = ft_strjoin_char(arg->value, cmd->segment[arg->i]);
			what_quote(cmd->segment, arg);
		}
		else if (add_char(cmd->segment, arg))
			return (free(arg->value), -1);
	}
	// printf("value: {%s}\n", arg->value);
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

int only_redirect(char *str, int i)
{
	int n;
	n = i;
	// printf("str[%d]: {%c}\n", n, str[n]);
	while (str[n] && str[n] != '=')
		n++;
	if (!str[n])
		return (0);
	n++;
	n++;
	if ((str[n] == '>' || str[n] == '<') || str[n] == '|')
		return (1);
	return (0);
}

int	handle_arg(t_shell *mini, t_cmd *cmd, int i, t_expand *arg, int *arg_index)
{
	// printf("cmd->segment[%d]: {%c}\n", i, cmd->segment[i]);
	if (ft_strcmp(cmd->command, "export") == 0 && only_redirect(cmd->segment, i))
		i = export_args(mini, cmd, arg, i);
	if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
		i = arg_in_quotes(mini, cmd->segment, i, arg);
	else
		i = arg_no_quotes(mini, cmd, arg, i);
	if (i == -1)
		return (-1);
	// printf("value: {%s}\nsegment: {%c}\n", arg->value, cmd->segment[i]);
	if (!arg->value || append_to_array(cmd, arg->value, arg->len, arg_index) == -1)
	{
		free(arg->value);
		ft_free_array(cmd->args);
		return (-1);
	}
	// free(arg->value);
	arg->value = ft_strdup("");
	i = skip_whitespace(cmd->segment, i);
	return (i);
}

int	handle_cmd_args(t_shell *mini, t_cmd *cmd, int i)
{
	int			arg_index;
	t_expand	arg;

	if (init_args_array(cmd, i) == -1)
		return (-1);
	arg_index = 1;
	i = skip_whitespace(cmd->segment, i);
	the_arg(&arg, i);
	while (cmd->segment[i] && arg_index < cmd->args_count + 1)
	{
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(mini, cmd, i);
			if (i == -1)
				return (-1);
			i = skip_whitespace(cmd->segment, i);
			continue;
		}
		i = handle_arg(mini, cmd, i, &arg, &arg_index);
		if (i == -1)
			return (-1);
		i = skip_whitespace(cmd->segment, i);
	}
	cmd->args[arg_index] = NULL;
	return (i);
}
