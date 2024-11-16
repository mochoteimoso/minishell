/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:58:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/16 18:29:36 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		create_pipe(int pipe_fd[2]);
int		dup_input(t_cmd *cmd, int pipe_fd[2], int i);
int		dup_output(t_cmd *cmd, int pipe_fd[2], int count, int i);
int		dup2_and_close(int old_fd, int new_fd);
void	close_pipe_fds(int pipe_fd[2]);

// Creates pipes for the pipeline

int	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

// Duplicates input from fd or pipe_fd

int	dup_input(t_cmd *cmd, int pipe_fd[2], int i)
{
	//printf("Value of i in input: %d\n", i);
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2_and_close(cmd->fd_in, STDIN_FILENO))
		{
			close(cmd->fd_in);
			close_pipe_fds(pipe_fd);
			return (1);
		}
		//printf("Input redirection handled for cmd[%d].\n", i);
	}
	else if (i > 0) // Use pipe for input
	{
		if (dup2_and_close(pipe_fd[0], STDIN_FILENO))
		{
			close(cmd->fd_out);
			close_pipe_fds(pipe_fd);
			return (1);
		}
		 //printf("Pipe input handled for cmd[%d].\n", i);
	}
	return (0);
}

// Duplicates output to fd or pipe_fd

int	dup_output(t_cmd *cmd, int pipe_fd[2], int count, int i)
{
	//printf("Value of i in output: %d\n", i);
	//printf("Value of count: %d\n", count);
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2_and_close(cmd->fd_out, STDOUT_FILENO))
		{
			close(cmd->fd_out);
			close_pipe_fds(pipe_fd);
			return (1);
		}
		//printf("Output redirection handled for cmd[%d].\n", i);
	}
	else if (i < count - 1) // Use pipe for output
	{
		if (dup2_and_close(pipe_fd[1], STDOUT_FILENO))
		{
			close(cmd->fd_out);
			close_pipe_fds(pipe_fd);
			return (1);
		}
		//printf("Pipe output handled for cmd[%d].\n", i);
	}
	return (0);
}

// Helper function to duplicate and close fd

int	dup2_and_close(int old_fd, int new_fd)
{
	if (old_fd < 0)
		return (1);
	if (dup2(old_fd, new_fd) == -1)
	{
		close(old_fd);
		perror("dup2");
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
