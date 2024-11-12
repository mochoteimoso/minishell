/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:05:32 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/12 10:51:19 by henbuska         ###   ########.fr       */
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
	if ((cmd->segment[i] == '>' || cmd->segment[i] == '<') && 
		!is_in_quotes(cmd->segment, i))
		return (true);
	else
		return (false);
}

// Handles < redirection, finds the filename and copies data to the redir linked list  

int	handle_redirect_in(t_cmd *cmd, int i)
{
	char	*filename_start;
	int		filename_length;
	
	filename_length = 0;
	i++;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	filename_start = &cmd->segment[i] ;
	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) && !is_redirection(cmd, i) &&
		cmd->segment[i] != '|' && cmd->segment[i] != '$')
	{
		filename_length++;
		i++;
	}
	cmd->redir_tail->file = ft_strndup(filename_start, filename_length);
	if (!cmd->redir_tail->file)
	{
		printf("Failed to allocate memory for filename\n");
		return (-1);
	}
	cmd->redir_tail->type = REDIRECT_IN;
	return (i);
}

// Handles > redirection, finds the filename and copies data to the redir linked list  

int	handle_redirect_out(t_cmd *cmd, int i)
{
	char	*filename_start;
	int		filename_length;
	
	filename_length = 0;
	i++;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	filename_start = &cmd->segment[i] ;
	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) && !is_redirection(cmd, i) &&
		cmd->segment[i] != '|' &&cmd->segment[i] != '$')
	{
		filename_length++;
		i++;
	}
	cmd->redir_tail->file = ft_strndup(filename_start, filename_length);
	if (!cmd->redir_tail->file)
	{
		printf("Failed to allocate memory for filename\n");
		return (-1);
	}
	//printf("output file after copy: %s\n", cmd->redir_tail->file);
	cmd->redir_tail->type = REDIRECT_OUT;
	return (i);
}

// Handles heredoc, finds the delimiter and copies data to the redir linked list  

int	handle_heredoc(t_cmd *cmd, int i)

{
	char	*delimiter_start;
	int		delimiter_length;

	delimiter_length = 0;
	i++;
	i++;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	delimiter_start = &cmd->segment[i];
	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) && !is_redirection(cmd, i) &&
		cmd->segment[i] != '|' && cmd->segment[i] != '$')
	{
		delimiter_length++;
		i++;
	}
	cmd->redir_tail->delimiter = ft_strndup(delimiter_start, delimiter_length);
	if (!cmd->redir_tail->delimiter)
	{
		printf("Failed to allocate memory for heredoc delimiter\n");
		return (-1);
	}
	//printf("heredoc_delim after copy: %s\n", sh->cmds[index]->heredoc_delim);
	cmd->redir_tail->type = HEREDOC;
	return (i);
}

// Handles append redirection, finds the filename and copies data to the redir linked list  

int	handle_append(t_cmd *cmd, int i)
{
	char	*filename_start;
	int		filename_length;
	
	filename_length = 0;
	i++;
	i++;
	while (cmd->segment[i]&& ft_isspace(cmd->segment[i]))
		i++;
	filename_start = &cmd->segment[i];
	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) && !is_redirection(cmd, i) &&
		cmd->segment[i] != '|' &&cmd->segment[i] != '$')
	{
		filename_length++;
		i++;
	}
	cmd->redir_tail->file = ft_strndup(filename_start, filename_length);
	if (!cmd->redir_tail->file)
	{
		printf("Failed to allocate memory for filename\n");
		return (-1);
	}
	cmd->redir_tail->type = APPEND;
	return (i);
}
