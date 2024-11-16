/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:10:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/16 15:20:50 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	resolve_fd(t_cmd *cmd);
int resolve_input_fd(t_cmd *cmd);
int resolve_output_fd(t_cmd *cmd);

// Figures out fds for input and output

int	resolve_fd(t_cmd *cmd)
{
	int	fd_in;
	int	fd_out;
	
	fd_in = -1;
	fd_out = -1;	
	fd_in = resolve_input_fd(cmd);
	//if (fd_in == -1 && cmd->redir_head)
	//	return (EXIT_FAILURE);
	if (fd_in != -1)
		cmd->fd_in = fd_in;
	else
		cmd->fd_in = STDIN_FILENO;
	fd_out = resolve_output_fd(cmd);
	//if (fd_out == -1 && cmd->redir_head)
	//	if (fd_in != -1)
	//		close(fd_in);
	//	return (EXIT_FAILURE);
	//}
	if (fd_out != -1)
		cmd->fd_out = fd_out;
	else
		cmd->fd_out = STDOUT_FILENO;
	return (0);
}

// Loops through redirection list to find last input fd

int	resolve_input_fd(t_cmd *cmd)
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
				close(fd_in);
			if (temp->type == 0)
				fd_in = open_input_file(temp->file);
			else if (temp->type == 3)
				fd_in = open_heredoc(temp->delimiter);
			if (fd_in == -1)
				return (-1);
		}
		temp = temp->next;
	}
	return (fd_in);
}

// Loops through redirection list to find last output fd

int	resolve_output_fd(t_cmd *cmd)
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
				close(fd_out);
			if (temp->type == 1)
				fd_out = open_output_file(temp->file);
			else if (temp->type == 2)
				fd_out = open_append_file(temp->file);
			if (fd_out == -1)
				return (-1);
		}
		temp = temp->next;
	}
	return (fd_out);
}
