/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:01:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 12:20:56 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_redirection(t_cmd *cmd, int i);
int		handle_redirect_in(t_cmd *cmd, int i);
int		handle_redirect_out(t_cmd *cmd, int i);
int		handle_heredoc(t_shell *mini, t_cmd *cmd, int i);
int		handle_append(t_cmd *cmd, int i);

// Checks if the input contains a redirection symbol that is not within quotes
bool	is_redirection(t_cmd *cmd, int i)
{
	if ((cmd->seg[i] == '>' || cmd->seg[i] == '<')
		&& !check_quotes(cmd->seg, i))
		return (true);
	else
		return (false);
}

// Handles < redirection, finds the filename and copies data
// to the redir linked list
int	handle_redirect_in(t_cmd *cmd, int i)
{
	char	*filename;

	filename = NULL;
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
	char	*filename;

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

int	handle_heredoc(t_shell *mini, t_cmd *cmd, int i)
{
	char	*delim;

	delim = NULL;
	i += 2;
	if (cmd->seg[i] == '\'' || cmd->seg[i] == '"')
		cmd->redir_tail->expand = false;
	i = parse_filename(cmd, i, &delim);
	if (i == -1 || !delim)
		return (-1);
	cmd->redir_tail->delimiter = delim;
	cmd->redir_tail->type = HEREDOC;
	if (generate_hd_file(cmd))
		return (-1);
	if (open_and_write_to_heredoc(mini, cmd))
		return (-1);
	return (i);
}

/* Handles append redirection, finds the filename and copies data
to the redir linked list*/
int	handle_append(t_cmd *cmd, int i)
{
	char	*filename;

	filename = NULL;
	i += 2;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = APPEND;
	return (i);
}
