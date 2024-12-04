/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 11:43:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/04 16:33:33 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_handler(t_shell *mini, int i, int exit_status)
{
	if (mini->cmd_count > 2 && i == mini->cmd_count - 1)
		close(mini->pipes[i - 1][0]);
	if (mini->cmd_count >= 2 && i < mini->cmd_count - 1)
	{
		close(mini->pipes[i][0]);
		close(mini->pipes[i][1]);
	}
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
	//free(mini);
	//mini = NULL;
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
	//free(mini);
	//mini = NULL;
	exit (exit_status);
}

void	exit_for_single_cmd(t_shell *mini, int exit_status)
{
	clean_env(mini->env, mini->pending);
	clean_cmds(mini->cmds);
	free(mini->pids);
	mini->pids = NULL;
	//free(mini);
	//mini = NULL;
	exit (exit_status);
}

void	cleaner_for_main(t_shell *mini)
{
	clean_cmds(mini->cmds);
	free(mini->pids);
	mini->pids = NULL;
	//free(mini);    //when should this be freed?
	//mini = NULL;
}

