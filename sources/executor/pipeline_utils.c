/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:01:57 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/25 18:17:26 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_fds_and_pipes(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i);
void	wait_children(t_shell *mini);

// Closes current command's fds, previous pipe and pipe_fds when needed

void	close_fds_and_pipes(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	if (i > 0)
	{
		close(mini->prev_pipe);
		//mini->prev_pipe = -1;
	}
	
	if (i < mini->cmd_count - 1)
	{
		mini->prev_pipe = pipe_fd[0];  // Store the current pipe's read end for the next command
		close(pipe_fd[1]);
	}
}

// Waits for all child prcocesses to complete and catches their exit status

void	wait_children(t_shell *mini)
{
	int	i;
	int	status;
	
	i = 0;
	status = 0;
	while (i < mini->cmd_count)
	{
		waitpid(mini->pids[i], &status, 0);
		if (WIFEXITED(status))
			mini->exit_stat = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			printf("Child %d terminated by signal %d\n", mini->pids[i], WTERMSIG(status));
		i++;
	}
}

