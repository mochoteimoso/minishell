/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:18:31 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/26 15:49:16 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		mini_cleaner(t_shell *mini);
void		clean_cmds(t_cmd **cmds);
void		cleaner_for_success(t_shell *mini);
void		cleaner_for_failure(t_shell *mini);
static void	free_and_close_pipes(t_shell *mini);

void	mini_cleaner(t_shell *mini)
{
	if (mini->env)
		clean_env(mini->env, NULL);
	if (mini->pending)
		ft_free_array(mini->pending);
	if (mini->cmds)
		clean_cmds(mini->cmds);
	if (mini->pids)
		free(mini->pids);
	if (mini->cmd_count > 1)
		ft_free_int_arr_with_size(mini->pipes, mini->cmd_count - 1);
}

void	clean_cmds(t_cmd **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		free(cmds[i]->seg);
		if (cmds[i]->command != NULL)
			free(cmds[i]->command);
		if (cmds[i]->cmd_path && cmds[i]->cmd_path != cmds[i]->command)
			free(cmds[i]->cmd_path);
		ft_free_array(cmds[i]->args);
		if (cmds[i]->redir_head)
			clean_redir(cmds[i]->redir_head);
		free(cmds[i]);
		i++;
	}
	free(cmds);
	cmds = NULL;
}

void	cleaner_for_success(t_shell *mini)
{
	int	i;

	i = mini->cmd_count - 2;
	clean_cmds(mini->cmds);
	if (mini->pids)
	{
		free(mini->pids);
		mini->pids = NULL;
	}
	if (mini->cmd_count > 1 && mini->pipes)
	{
		if (mini->pipes[i][0] != -1)
		{
			if (mini->pipes[i][0] > 0)
				close(mini->pipes[i][0]);
		}
	}
	if (mini->cmd_count > 1)
		ft_free_int_arr_with_size(mini->pipes, mini->cmd_count - 1);
}

void	cleaner_for_failure(t_shell *mini)
{
	clean_cmds(mini->cmds);
	if (mini->pids)
	{
		free(mini->pids);
		mini->pids = NULL;
	}
	if (mini->cmd_count > 1 && mini->pipes)
		free_and_close_pipes(mini);
}

static void	free_and_close_pipes(t_shell *mini)
{
	int	i;

	i = 0;
	while (i < mini->cmd_count - 1)
	{
		if (mini->pipes[i][0] > 0)
			close(mini->pipes[i][0]);
		if (mini->pipes[i][1] > 1)
			close(mini->pipes[i][1]);
		free(mini->pipes[i]);
		i++;
	}
	free(mini->pipes);
	mini->pipes = NULL;
}
