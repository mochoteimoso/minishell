/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:58:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/03 09:47:45 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		dup2_and_close_in_main(t_shell *mini, int old_fd, int new_fd);
//void	close_fds_and_pipes(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i);
void	close_fds_and_pipes(t_shell *mini, int i);
void	wait_children(t_shell *mini);

// Duplicates fds in main process and resets fds to STDIN/STDOUT when needed
// only difference is that this resets fds in case of error here to save space

int	dup2_and_close_in_main(t_shell *mini, int old_fd, int new_fd)
{
	if (old_fd < 0)
	{
		ft_putendl_fd("Invalid file descriptor", 2);
		reset_fds(mini);
		return (1);
	}
	if (dup2(old_fd, new_fd) == -1)
	{
		close(old_fd);
		reset_fds(mini);
		return (1);
	}
	close(old_fd);
	return (0);
}

void	close_fds_and_pipes(t_shell *mini, int i)
{
	if (mini->cmd_count > 1)
	{
		// For the first command
		if (i == 0)
		{
			close(mini->pipes[i][1]); // Close write end of the first pipe
		}
		// For middle commands
		else if (i < mini->cmd_count - 1)
		{
			close(mini->pipes[i - 1][0]); // Close read end of the previous pipe
			close(mini->pipes[i][1]);    // Close write end of the current pipe
		}
		// For the last command
		else if (i == mini->cmd_count - 1)
		{
			close(mini->pipes[i - 1][0]); // Close read end of the last pipe
		}
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
