/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:58:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 17:33:06 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	dup_input(t_shell *mini, t_cmd *cmd, int i);
int	dup_output(t_shell *mini, t_cmd *cmd, int count, int i);
int	dup2_and_close(int old_fd, int new_fd);

/**
 * dup_input - Redirects input for a command.
 *
 * @mini: Pointer to the shell structure containing environment details.
 * @cmd: Pointer to the command structure containing command details.
 * @i: Index of the command in the pipeline.
 *
 * Handles input redirection by duplicating `cmd->fd_in` to `STDIN_FILENO`.
 * If no input redirection is specified, duplicates the read end of the
 * previous pipeline to `STDIN_FILENO`. Closes unnecessary file descriptors.
 * Returns 0 on success, or 1 if duplication fails.
 */
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
	else if (i > 0)
	{
		if (dup2_and_close(mini->pipes[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 for pipe input");
			cmd->cmd_exit = 1;
			return (1);
		}
	}
	return (0);
}

/**
 * dup_output - Redirects output for a command.
 *
 * @mini: Pointer to the shell structure containing environment details.
 * @cmd: Pointer to the command structure containing command details.
 * @count: Total number of commands in the pipeline.
 * @i: Index of the command in the pipeline.
 *
 * Handles output redirection by duplicating `cmd->fd_out` to `STDOUT_FILENO`.
 * If no output redirection is specified, duplicates the write end of the
 * current pipeline to `STDOUT_FILENO`. Closes unnecessary file descriptors.
 * Returns 0 on success, or 1 if duplication fails.
 */
int	dup_output(t_shell *mini, t_cmd *cmd, int count, int i)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
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
		if (dup2_and_close(mini->pipes[i][1], STDOUT_FILENO))
		{
			perror("dup2 for output to pipe");
			cmd->cmd_exit = 1;
			return (1);
		}
	}
	return (0);
}

/**
 * dup2_and_close - Duplicates a file descriptor and closes the old one.
 *
 * @old_fd: File descriptor to duplicate.
 * @new_fd: Target file descriptor for duplication.
 *
 * Duplicates `old_fd` to `new_fd` using `dup2`.
 * Closes `old_fd` after duplication. If `old_fd` is invalid or `dup2` fails,
 * it prints an error message and returns 1.
 *
 * Returns 0 on success.
 */
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
