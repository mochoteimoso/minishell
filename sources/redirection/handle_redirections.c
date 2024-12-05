/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:05:32 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/05 17:52:54 by henbuska         ###   ########.fr       */
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

	filename = NULL;
	in_quotes = false;
	i++;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = REDIRECT_IN;
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
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = REDIRECT_OUT;
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
