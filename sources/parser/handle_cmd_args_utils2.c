/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 14:49:34 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/26 14:45:12 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_args(t_cmd *cmd, int i);
int	count_if_redirection(t_cmd *cmd, int i);
int	only_redirect(char *str, int i);
int	init_args_array(t_cmd *cmd, int i);

int	count_args(t_cmd *cmd, int i)
{
	int	args_count;

	args_count = 0;
	while (cmd->seg[i] && ft_isspace(cmd->seg[i]))
		i++;
	while (cmd->seg[i])
	{
		if (is_redirection(cmd, i))
		{
			i++;
			i = count_if_redirection(cmd, i);
		}
		else
		{
			args_count++;
			while (cmd->seg[i] && !ft_isspace(cmd->seg[i])
				&& !is_redirection(cmd, i))
				i++;
		}
		while (cmd->seg[i] && ft_isspace(cmd->seg[i]))
			i++;
	}
	return (args_count);
}

int	count_if_redirection(t_cmd *cmd, int i)
{
	while (cmd->seg[i] && ft_isspace(cmd->seg[i]))
		i++;
	while (cmd->seg[i] && !ft_isspace(cmd->seg[i])
		&& is_redirection(cmd, i))
		i++;
	return (i);
}

int	only_redirect(char *str, int i)
{
	int	n;

	n = i;
	while (str[n] && str[n] != '=')
		n++;
	if (!str[n])
		return (0);
	n++;
	if (str[n])
		n++;
	if ((str[n] == '>' || str[n] == '<') || str[n] == '|')
		return (1);
	return (0);
}

int	init_args_array(t_cmd *cmd, int i)
{
	cmd->a_num = count_args(cmd, i) + 1;
	cmd->args = ft_calloc(cmd->a_num + 1, sizeof(char *));
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
