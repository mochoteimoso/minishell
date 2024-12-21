/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:18:09 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/21 16:35:46 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int only_redirect(char *str, int i)
{
	int n;
	n = i;
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

int	count_if_redirection(t_cmd *cmd, int i)
{
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) &&
		is_redirection(cmd, i))
		i++;
	return (i);
}

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