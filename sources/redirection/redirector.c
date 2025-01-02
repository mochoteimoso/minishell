/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:10:23 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:56:07 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			resolve_fd(t_cmd *cmd);
static int	process_redirections(t_cmd *cmd, int *fd_in, int *fd_out);
static int	resolve_input_fd(t_cmd *cmd, t_redir *redir, int *fd_in);
static int	resolve_output_fd(t_cmd *cmd, t_redir *redir, int *fd_out);
static void	close_fd_if_needed(int fd);

/**
 * resolve_fd - Resolves the file descriptors for input
 * 				and output redirections in a command.
 *
 * @cmd: Pointer to the command structure.
 *
 * Uses `process_redirections` to process all redirections and sets the command's
 * `fd_in` and `fd_out` to the appropriate file descriptors
 * or defaults to standard input/output if no redirection exists.
 * Closes previously opened file descriptors.
 *
 * Returns 0 on success,
 * or 1 on failure (e.g., invalid files or permission errors).
 */
int	resolve_fd(t_cmd *cmd)
{
	int	fd_in;
	int	fd_out;

	fd_in = -1;
	fd_out = -1;
	if (process_redirections(cmd, &fd_in, &fd_out))
		return (1);
	if (fd_in != -1)
		cmd->fd_in = fd_in;
	else
		cmd->fd_in = STDIN_FILENO;
	if (fd_out != -1)
		cmd->fd_out = fd_out;
	else
		cmd->fd_out = STDOUT_FILENO;
	return (0);
}

/**
 * process_redirections - Processes all redirections for a command.
 *
 * @cmd: Pointer to the command structure containing redirection details.
 * @fd_in: Pointer to the input file descriptor.
 * @fd_out: Pointer to the output file descriptor.
 *
 * Iterates over the redirection list (`redir_head`) in the command and resolves
 * input and output redirections using `resolve_input_fd`
 * and `resolve_output_fd`.
 * Closes the file descriptors if an error occurs during resolution.
 *
 * Returns 0 on success, or 1 on failure.
 */
int	process_redirections(t_cmd *cmd, int *fd_in, int *fd_out)
{
	t_redir	*temp;

	temp = cmd->redir_head;
	while (temp)
	{
		if (temp->type == 0 || temp->type == 3)
		{
			if (resolve_input_fd(cmd, temp, fd_in))
			{
				close_fd_if_needed(*fd_out);
				return (1);
			}
		}
		else if (temp->type == 1 || temp->type == 2)
		{
			if (resolve_output_fd(cmd, temp, fd_out))
			{
				close_fd_if_needed(*fd_in);
				return (1);
			}
		}
		temp = temp->next;
	}
	return (0);
}

/**
 * resolve_input_fd - Resolves the input file descriptor for a redirection.
 *
 * @cmd: Pointer to the command structure.
 * @redir: Pointer to the redirection structure.
 * @fd_in: Pointer to the input file descriptor.
 *
 * Closes any previously opened input file descriptor. Opens the file or heredoc
 * associated with the redirection. Updates `fd_in` with the new file descriptor.
 *
 * Returns 0 on success,
 * or 1 on failure (e.g., file not found, permission denied).
 */
int	resolve_input_fd(t_cmd *cmd, t_redir *redir, int *fd_in)
{
	if (*fd_in > 2)
	{
		close(*fd_in);
		*fd_in = -1;
	}
	if (redir->type == 0)
		*fd_in = open_input_file(cmd, redir->file);
	else if (redir->type == 3)
		*fd_in = open_heredoc(cmd, redir->heredoc_name);
	if (*fd_in == -1)
		return (1);
	return (0);
}

/**
 * resolve_output_fd - Resolves the output file descriptor for a redirection.
 *
 * @cmd: Pointer to the command structure.
 * @redir: Pointer to the redirection structure.
 * @fd_out: Pointer to the output file descriptor.
 *
 * Closes any previously opened output file descriptor.
 * Opens the file for writing or appending, depending on the redirection type.
 * Updates `fd_out` with the new file descriptor.
 *
 * Returns 0 on success, or 1 on failure (e.g., permission denied, invalid file).
 */
int	resolve_output_fd(t_cmd *cmd, t_redir *redir, int *fd_out)
{
	if (*fd_out > 2)
	{
		close(*fd_out);
		*fd_out = -1;
	}
	if (redir->type == 1)
		*fd_out = open_output_file(cmd, redir->file);
	else if (redir->type == 2)
		*fd_out = open_append_file(cmd, redir->file);
	if (*fd_out == -1)
		return (1);
	return (0);
}

/**
 * close_fd_if_needed - Closes a file descriptor if it is valid.
 *
 * @fd: The file descriptor to close.
 *
 * Ensures the file descriptor is greater than 2
 * (not stdin, stdout, or stderr) before closing.
 */
static void	close_fd_if_needed(int fd)
{
	if (fd > 2)
		close(fd);
}
