/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 11:43:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/30 17:25:56 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_for_failure(t_shell *mini, int i, int exit_status);
void	exit_for_success(t_shell *mini, int i, int exit_status);
void	exit_for_single_cmd(t_shell *mini, int exit_status);
void	close_all_pipes(t_shell *mini);
void	hd_free(t_expand *arg, char *expan);

void	exit_for_failure(t_shell *mini, int i, int exit_status)
{
	close_all_pipes(mini);
	/*if (mini->cmd_count >= 2 && i < mini->cmd_count - 1)
	{
		close(mini->pipes[i][0]);
		close(mini->pipes[i][1]);
	}*/
	if (mini->cmds[i]->fd_out > 2)
		close(mini->cmds[i]->fd_out);
	if (mini->cmds[i]->fd_in > 2)
		close(mini->cmds[i]->fd_in);
	clean_env(mini->env, mini->pending);
	clean_cmds(mini->cmds);
	free(mini->pids);
	mini->pids = NULL;
	if (mini->cmd_count > 1)
		ft_free_int_arr_with_size(mini->pipes, mini->cmd_count - 1);
	free(mini);
	mini = NULL;
	exit (exit_status);
}

void	exit_for_success(t_shell *mini, int i, int exit_status)
{
	if (mini->cmds[i]->fd_out != STDOUT_FILENO && mini->cmds[i]->fd_out != -1)
		close(mini->cmds[i]->fd_out);
	if (mini->cmds[i]->fd_in != STDIN_FILENO && mini->cmds[i]->fd_in != -1)
		close(mini->cmds[i]->fd_in);
	clean_env(mini->env, mini->pending);
	clean_cmds(mini->cmds);
	free(mini->pids);
	mini->pids = NULL;
	if (mini->cmd_count > 1)
		ft_free_int_arr_with_size(mini->pipes, mini->cmd_count - 1);
	free(mini);
	mini = NULL;
	exit (exit_status);
}

void	exit_for_single_cmd(t_shell *mini, int exit_status)
{
	clean_env(mini->env, mini->pending);
	clean_cmds(mini->cmds);
	free(mini->pids);
	mini->pids = NULL;
	exit (exit_status);
}

void	hd_free(t_expand *arg, char *expan)
{
	free(arg->value);
	free(expan);
}

void	close_all_pipes(t_shell *mini)
{
	int	j;

	j = 0;
	while (j < mini->cmd_count - 1)
	{
		close(mini->pipes[j][0]);
		close(mini->pipes[j][1]);
		j++;
	}
}
