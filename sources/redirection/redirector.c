/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:10:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/12 14:59:02 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	resolve_fd(t_cmd *cmd);
int	process_redirections(t_cmd *cmd, int *fd_in, int *fd_out);
int	resolve_input_fd(t_cmd *cmd, t_redir *redir, int *fd_in);
int	resolve_output_fd(t_cmd *cmd, t_redir *redir, int *fd_out);
//static int	resolve_input_fd(t_cmd *cmd);
//static int	resolve_output_fd(t_cmd *cmd);

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
				if (*fd_out > 2)
					close(*fd_out);
				return (1);
			}
		}
		else if (temp->type == 1 || temp->type == 2)
		{
			if (resolve_output_fd(cmd, temp, fd_out))
			{
				if (*fd_in > 2)
					close(*fd_in);
				return (1);
			}
		}
		temp = temp->next;
	}
	return (0);
}

int	resolve_input_fd(t_cmd *cmd, t_redir *redir, int *fd_in)
{
	if (*fd_in != -1)
	{
		close(*fd_in);
		*fd_in = -1;
	}
	if (redir->type == 0)
		*fd_in = open_input_file(cmd, redir->file);
	else if (redir->type == 3)
		*fd_in = open_heredoc(cmd, redir->delimiter);
	if (*fd_in == -2)
		return (1);
	return (0);
}

int	resolve_output_fd(t_cmd *cmd, t_redir *redir, int *fd_out)
{
	if (*fd_out != -1)
	{
		close(*fd_out);
		*fd_out = -1;
	}
	if (redir->type == 1)
		*fd_out = open_output_file(cmd, redir->file);
	else if (redir->type == 2)
		*fd_out = open_append_file(cmd, redir->file);
	if (*fd_out == -2)
		return (1);
	return (0);
}

/*int	resolve_fd(t_cmd *cmd)
{
	int	fd_in;
	int	fd_out;

	
	fd_in = resolve_input_fd(cmd);
	//if (fd_in == -2)
	//	return (1);
	if (fd_in != -1)
		cmd->fd_in = fd_in;
	else
		cmd->fd_in = STDIN_FILENO;
	fd_out = resolve_output_fd(cmd);
	if (fd_out == -2)
		return (1);
	if (fd_out != -1)
		cmd->fd_out = fd_out;
	else
		cmd->fd_out = STDOUT_FILENO;
	if (fd_in == -2)
		return (1);
	return (0);
}

// Loops through redirection list to find last input fd

static int	resolve_input_fd(t_cmd *cmd)
{
	int		fd_in;
	t_redir	*temp;

	fd_in = -1;
	temp = cmd->redir_head;
	while (temp)
	{
		if (temp->type == 0 || temp->type == 3)
		{
			if (fd_in != -1)
			{
				close(fd_in);
				fd_in = -1;
			}
			if (temp->type == 0)
				fd_in = open_input_file(cmd, temp->file);
			else if (temp->type == 3)
				fd_in = open_heredoc(cmd, temp->delimiter);
			if (fd_in == -2)
				return (-2);
		}
		temp = temp->next;
	}
	return (fd_in);
}

// Loops through redirection list to find last output fd

static int	resolve_output_fd(t_cmd *cmd)
{
	int		fd_out;
	t_redir	*temp;

	fd_out = -1;
	temp = cmd->redir_head;
	while (temp)
	{
		if (temp->type == 1 || temp->type == 2)
		{
			if (fd_out != -1)
			{
				close(fd_out);
				fd_out = -1;
			}
			if (temp->type == 1)
				fd_out = open_output_file(cmd, temp->file);
			else if (temp->type == 2)
				fd_out = open_append_file(cmd, temp->file);
			if (fd_out == -2)
				return (-2);
		}
		temp = temp->next;
	}
	return (fd_out);
} */
