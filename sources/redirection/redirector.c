/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:10:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/28 11:58:30 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			resolve_fd(t_cmd *cmd);
static int	process_redirections(t_cmd *cmd, int *fd_in, int *fd_out);
static int	resolve_input_fd(t_cmd *cmd, t_redir *redir, int *fd_in);
static int	resolve_output_fd(t_cmd *cmd, t_redir *redir, int *fd_out);
static void	close_fd_if_needed(int fd);

// Deduces fds for input and output

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

static void	close_fd_if_needed(int fd)
{
	if (fd > 2)
		close(fd);
}
