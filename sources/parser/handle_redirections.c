/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:05:32 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/11 11:53:11 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_redirection(t_cmd *cmd, int i);
int		handle_redirect_in(t_cmd *cmd, int i, t_redir *redir_node);
int		handle_redirect_out(t_cmd *cmd, int i, t_redir *redir_node);
int		handle_heredoc(t_cmd *cmd, int i, t_redir *redir_node);
int		handle_append(t_cmd *cmd, int i, t_redir *redir_node);

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

int	handle_redirect_in(t_cmd *cmd, int i, t_redir *temp)
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
	temp->file = ft_strndup(filename_start, filename_length);
	if (!temp->file)
	{
		printf("Failed to allocate memory for filename\n");
		return (-1);
	}
	temp->type = REDIRECT_IN;
	return (i);
}

// Handles > redirection, finds the filename and copies data to the redir linked list  

int	handle_redirect_out(t_cmd *cmd, int i, t_redir *redir_node)
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
	redir_node->file = ft_strndup(filename_start, filename_length);
	if (!redir_node->file)
	{
		printf("Failed to allocate memory for filename\n");
		return (1);
	}
	redir_node->type = REDIRECT_OUT;
	return (i);
}

// Handles heredoc, finds the delimiter and copies data to the redir linked list  

int	handle_heredoc(t_cmd *cmd, int i, t_redir *redir_node)
{
	char	*delimiter_start;
	int		delimiter_length;

	delimiter_length = 0;
	i++;
	i++;
	while (cmd->segment[i]&& ft_isspace(cmd->segment[i]))
		i++;
	delimiter_start = &cmd->segment[i];
	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) && !is_redirection(cmd, i) &&
		cmd->segment[i] != '|' &&cmd->segment[i] != '$')
	{
		delimiter_length++;
		i++;
	}
	redir_node->delimiter = ft_strndup(delimiter_start, delimiter_length);
	if (!redir_node->delimiter )
	{
		printf("Failed to allocate memory for heredoc delimiter\n");
		return (1);
	}
	//printf("heredoc_delim after copy: %s\n", sh->cmds[index]->heredoc_delim);
	redir_node->type = HEREDOC;
	return (0);
}

// Handles append redirection, finds the filename and copies data to the redir linked list  

int	handle_append(t_cmd *cmd, int i, t_redir *redir_node)
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
	redir_node->file = ft_strndup(filename_start, filename_length);
	if (!redir_node->file)
	{
		printf("Failed to allocate memory for filename\n");
		return (1);
	}
	redir_node->type = APPEND;
	return (0);
}
