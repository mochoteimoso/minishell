/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:58:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/20 11:34:40 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		dup_input(t_shell *mini, t_cmd *cmd, int i);
int		dup_output(t_cmd *cmd, int pipe_fd[2], int count, int i);
int		dup2_and_close(int old_fd, int new_fd);
void	close_pipe_fds(int pipe_fd[2]);
void	close_pipes(t_shell *mini, int pipe_fd[2]);

// Duplicates input from fd or pipe_fd

int	dup_input(t_shell *mini, t_cmd *cmd, int i)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		//printf("Redirecting input: cmd->fd_in = %d\n", cmd->fd_in);
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 for input redirection");
			return (1);
		}
		//printf("Input redirection handled for cmd[%d].\n", i);
	}
	else if (i > 0) // Middle or last command
	{
		//printf("Using pipe for input: pipe_fd[0] = %d\n", mini->prev_pipe[0]);
		if (dup2(mini->prev_pipe[0], STDIN_FILENO) == -1)
		{
			perror("dup2 for pipe input");
			return (1);
		}
		//printf("Pipe input handled for cmd[%d].\n", i);
	}
	return (0);
}

// Duplicates output to fd or pipe_fd

int	dup_output(t_cmd *cmd, int pipe_fd[2], int count, int i)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
		//printf("Redirecting output: cmd->fd_out = %d\n", cmd->fd_out);
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 for output redirection");
			return (1);
		}
	}
	else if (i < count - 1)
	{
		//printf("Using pipe for output: pipe_fd[1] = %d\n", pipe_fd[1]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 for restoring stdout");
			return (1);
		}
	}
	return (0);
}

// Helper function to duplicate and close fd

int	dup2_and_close(int old_fd, int new_fd)
{
	if (old_fd < 0)
	{
		fprintf(stderr, "Invalid file descriptor: %d\n", old_fd);
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

// Close all pipes in the array

void	close_pipe_fds(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	close_pipes(t_shell *mini, int pipe_fd[2])
{
	if (mini->prev_pipe[0] != -1)
		close(mini->prev_pipe[0]);
	if (pipe_fd[1] > 1)
		close(pipe_fd[1]);
}
