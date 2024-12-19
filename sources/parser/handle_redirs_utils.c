/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:01:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/19 19:57:11 by henbuska         ###   ########.fr       */
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
	if (!cmd->command)
	{
		temp_fd = open_input_file(cmd, cmd->redir_tail->file);
		close(temp_fd);
	}
	return (i);
}

// Handles > redirection, finds the filename and copies data 
// to the redir linked list

int	handle_redirect_out(t_cmd *cmd, int i)
{
	bool	in_quotes;
	char	*filename;
	int		temp_fd;

	in_quotes = false;
	filename = NULL;
	i++;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = REDIRECT_OUT;
	temp_fd = open_output_file(cmd, cmd->redir_tail->file);
	close(temp_fd);
	return (i);
}

// Handles heredoc, finds the delimiter and copies data to the redir linked list

int	handle_heredoc(t_shell *mini, t_cmd *cmd, int i)
{
	char	*delim;

	delim = NULL;
	i+=2;
	if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
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

// Handles append redirection, finds the filename and copies data 
// to the redir linked list

int	handle_append(t_cmd *cmd, int i)
{
	bool	in_quotes;
	char	*filename;
	int		temp_fd;

	in_quotes = false;
	filename = NULL;
	i+=2;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = APPEND;
	temp_fd = open_append_file(cmd, cmd->redir_tail->file);
	close(temp_fd);
	return (i);
}
