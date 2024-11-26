/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:58:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/26 11:32:29 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		dup_input(t_shell *mini, t_cmd *cmd, int i);
int		dup_output(t_cmd *cmd, int pipe_fd[2], int count, int i);
int		dup2_and_close(int old_fd, int new_fd);
void	close_unused_fds(t_shell *mini, t_cmd *cmd, int i);

// Duplicates input from fd_in if there is a redirection or from previous pipe
// Redirection takes precendence over pipe

int	dup_input(t_shell *mini, t_cmd *cmd, int i)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		//printf("Redirecting input: cmd->fd_in = %d\n", cmd->fd_in);
		if (dup2_and_close(cmd->fd_in, STDIN_FILENO))
		{
			perror("dup2 for input redirection");
			return (1);
		}
		if (mini->prev_pipe != -1)
			close(mini->prev_pipe);
		//printf("Input redirection handled for cmd[%d].\n", i);
	}
	else if (i > 0) // Middle or last command
	{
		//printf("Using pipe for input: pipe_fd[0] = %d\n", mini->prev_pipe);
		if (dup2_and_close(mini->prev_pipe, STDIN_FILENO) == -1)
		{
			perror("dup2 for pipe input");
			return (1);
		}
		//printf("Pipe input handled for cmd[%d].\n", i);
	}
	return (0);
}

// Duplicates output to fd if there is a redirection or to write end of pipe
// Redirection takes precendence over pipe

int	dup_output(t_cmd *cmd, int pipe_fd[2], int count, int i)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
		//printf("Redirecting output: cmd->fd_out = %d\n", cmd->fd_out);
		if (dup2_and_close(cmd->fd_out, STDOUT_FILENO))
		{
			perror("dup2 for output redirection");
			return (1);
		}
		if (i < count - 1)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
	}
	else if (i < count - 1)
	{
		//printf("Using pipe for output: pipe_fd[1] = %d\n", pipe_fd[1]);
		if (dup2_and_close(pipe_fd[1], STDOUT_FILENO))
		{
			perror("dup2 for restoring stdout");
			return (1);
		}
		close(pipe_fd[0]);
	}
	return (0);
}

// Helper function to duplicate and close fd

int	dup2_and_close(int old_fd, int new_fd)
{
	if (old_fd < 0)
	{
		ft_putendl_fd("Invalid file descriptor", 2);
		return (1);
	}
	if (dup2(old_fd, new_fd) == -1)
	{
		close(old_fd);
		return (1);
	}
	close(old_fd);
	return (0);
}

// Closes fds of the next commands in the pipeline that child 
// inherits from parent
// only the fds of the current command are used

void	close_unused_fds(t_shell *mini, t_cmd *cmd, int i)
{
	while (i < mini->cmd_count)
	{
		if (mini->cmds[i]->fd_in != -1 && mini->cmds[i]->fd_in != cmd->fd_in)
			close(mini->cmds[i]->fd_in);
		if (mini->cmds[i]->fd_out != -1 && mini->cmds[i]->fd_out != cmd->fd_out)
			close(mini->cmds[i]->fd_out);
		i++;
	}
}

