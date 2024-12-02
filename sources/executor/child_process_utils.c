/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:58:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/02 18:01:22 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		dup_input(t_shell *mini, t_cmd *cmd, int i);
//int	dup_output(t_cmd *cmd, int pipe_fd[2], int count, int i);
int		dup_output(t_shell *mini, t_cmd *cmd, int count, int i);
int		dup2_and_close(int old_fd, int new_fd);

// Duplicates input from fd_in if there is a redirection or from previous pipe
// Redirection takes precedence over pipe

/*int	dup_input(t_shell *mini, t_cmd *cmd, int i)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2_and_close(cmd->fd_in, STDIN_FILENO))
		{
			perror("dup2 for input redirection");
			cmd->cmd_exit = 1;
			return (1);
		}
	}
	else if (i > 0) // Middle or last command
	{
		if (dup2_and_close(mini->prev_pipe, STDIN_FILENO) == -1)
		{
			perror("dup2 for pipe input");
			cmd->cmd_exit = 1;
			return (1);
		}
		//printf("Pipe input handled for cmd[%d].\n", i);
	}
	return (0);
} */

int	dup_input(t_shell *mini, t_cmd *cmd, int i)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2_and_close(cmd->fd_in, STDIN_FILENO))
		{
			perror("dup2 for input redirection");
			cmd->cmd_exit = 1;
			return (1);
		}
		if (i > 0)
		close(mini->pipes[i - 1][0]);
	}
	else if (i > 0) // Middle or last command
	{
		if (dup2_and_close(mini->pipes[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 for pipe input");
			cmd->cmd_exit = 1;
			return (1);
		}
		//printf("Pipe input handled for cmd[%d].\n", i);
	}
	return (0);
}

// Duplicates output to fd if there is a redirection or to write end of pipe
// Redirection takes precedence over pipe

/*int	dup_output(t_cmd *cmd, int pipe_fd[2], int count, int i)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
		//printf("Redirecting output: cmd->fd_out = %d\n", cmd->fd_out);
		if (dup2_and_close(cmd->fd_out, STDOUT_FILENO))
		{
			perror("dup2 for output redirection");
			cmd->cmd_exit = 1;
			return (1);
		}
		if (i < count - 1)
			close(pipe_fd[1]);
	}
	else if (i < count - 1)
	{
		printf("Using pipe for output in cmd %d: pipe_fd[1] = %d\n", i, pipe_fd[1]);
		if (dup2_and_close(pipe_fd[1], STDOUT_FILENO))
		{
			perror("dup2 for output to pipe");
			cmd->cmd_exit = 1;
			return (1);
		}
		//close(pipe_fd[0]);
	}
	return (0);
} */

int	dup_output(t_shell *mini, t_cmd *cmd, int count, int i)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
		//printf("Redirecting output: cmd->fd_out = %d\n", cmd->fd_out);
		if (dup2_and_close(cmd->fd_out, STDOUT_FILENO))
		{
			perror("dup2 for output redirection");
			cmd->cmd_exit = 1;
			return (1);
		}
		if (i < count - 1)
			close(mini->pipes[i][1]);
	}
	else if (i < count - 1)
	{
		//printf("Using pipe for output in cmd %d: pipe_fd[1] = %d\n", i, pipe_fd[1]);
		if (dup2_and_close(mini->pipes[i][1], STDOUT_FILENO))
		{
			perror("dup2 for output to pipe");
			cmd->cmd_exit = 1;
			return (1);
		}
		//close(mini->pipes[i][0]);
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


