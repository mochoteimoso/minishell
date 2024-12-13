/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:01:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/13 10:26:12 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_redirection(t_cmd *cmd, int i);
int		handle_redirect_in(t_cmd *cmd, int i);
int		handle_redirect_out(t_cmd *cmd, int i);
int		handle_heredoc(t_cmd *cmd, int i);
int		handle_append(t_cmd *cmd, int i);

// Checks if the input contains a redirection symbol that is not within quotes

bool	is_redirection(t_cmd *cmd, int i)
{
	if ((cmd->segment[i] == '>' || cmd->segment[i] == '<')
		&& !check_quotes(cmd->segment, i))
		return (true);
	else
		return (false);
}

// Handles < redirection, finds the filename and copies data 
// to the redir linked list

int	handle_redirect_in(t_cmd *cmd, int i)
{
	char	*filename;
	bool	in_quotes;
	int		temp_fd;

	filename = NULL;
	in_quotes = false;
	i++;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = REDIRECT_IN;
	temp_fd = open_input_file(cmd, cmd->redir_tail->file);
	close(temp_fd);
	return (i);
}

// Handles > redirection, finds the filename and copies data 
// to the redir linked list

int	handle_redirect_out(t_cmd *cmd, int i)
{
	bool	in_quotes;
	char	*filename;

	in_quotes = false;
	filename = NULL;
	i++;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = REDIRECT_OUT;
	return (i);
}

// Handles heredoc, finds the delimiter and copies data to the redir linked list

int	handle_heredoc(t_cmd *cmd, int i)
{
	bool	in_quotes;
	char	*filename;

	in_quotes = false;
	filename = NULL;
	i+=2;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->delimiter = filename;
	cmd->redir_tail->type = HEREDOC;
	if (in_quotes)
		cmd->redir_tail->expand = false;
	else
		cmd->redir_tail->expand = true;
	return (i);
}

// Handles append redirection, finds the filename and copies data 
// to the redir linked list

int	handle_append(t_cmd *cmd, int i)
{
	bool	in_quotes;
	char	*filename;

	in_quotes = false;
	filename = NULL;
	i+=2;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = APPEND;
	return (i);
}
