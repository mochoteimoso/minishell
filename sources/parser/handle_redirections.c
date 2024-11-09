/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:05:32 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/09 18:08:12 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_redirection(t_cmd *cmd, int i);
int		handle_redirect_in(t_cmd *cmd, int *i);
int		handle_redirect_out(t_cmd *cmd, int *i);
int		handle_heredoc(t_cmd *cmd, int *i);
int		handle_append(t_cmd *cmd, int *i);

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

int	handle_redirect_in(t_cmd *cmd, int *i)
{
	char	*filename_start;
	int		filename_length;
	
	filename_length = 0;
	(*i)++;
	while (cmd->segment[*i] && ft_isspace(cmd->segment[*i]))
		(*i)++;
	filename_start = &cmd->segment[*i] ;
	while (cmd->segment[*i] && cmd->segment[*i] != ' ' && cmd->segment[*i] != '|' &&
		cmd->segment[*i] != '<' && cmd->segment[*i] != '>' && cmd->segment[*i] != '$')
	{
		filename_length++;
		(*i)++;
	}
	list_redir();
	mini->cmds[index]->redirect_in = ft_strndup(filename_start, filename_length);
	if (!mini->cmds[index]->redirect_in)
	{
		printf("Failed to allocate memory for filename\n");
		return (1);
	}
	mini->cmds[index]->redirect_type = REDIRECT_IN;
	return (0);
}

// Handles > redirection, finds the filename and copies data to the redir linked list  

int	handle_redirect_out(t_cmd *cmd, int *i)
{
	char	*filename_start;
	int		filename_length;
	
	filename_length = 0;
	(*i)++;
	while (cmd->segment[*i] && ft_isspace(cmd->segment[*i]))
		(*i)++;
	filename_start = &cmd->segment[*i];
	while (cmd->segment[*i] && cmd->segment[*i] != ' ' && cmd->segment[*i] != '|' &&
		cmd->segment[*i] != '<' && cmd->segment[*i] != '>' && cmd->segment[*i]!= '$')
	{
		filename_length++;
		(*i)++;
	}
	mini->cmds[index]->redirect_out = ft_strndup(filename_start, filename_length);
	if (!mini->cmds[index]->redirect_out)
	{
		printf("Failed to allocate memory for filename\n");
		return (1);
	}
	mini->cmds[index]->redirect_type = REDIRECT_OUT;
	return (0);
}

// Handles heredoc, finds the delimiter and copies data to the redir linked list  

int	handle_heredoc(t_cmd *cmd, int *i)
{
	char	*delimiter_start;
	int		delimiter_length;

	delimiter_length = 0;
	(*i)++;
	(*i)++;
	while (cmd->segment[*i]&& ft_isspace(cmd->segment[*i]))
		(*i)++;
	delimiter_start = &cmd->segment[*i];
	while (cmd->segment[*i] && cmd->segment[*i] != ' ' && cmd->segment[*i] != '|' &&
		cmd->segment[*i]!= '<' && cmd->segment[*i]!= '>' && cmd->segment[*i] != '$')
	{
		delimiter_length++;
		(*i)++;
	}
	mini->cmds[index]->heredoc_delim = ft_strndup(delimiter_start, delimiter_length);
	if (!mini->cmds[index]->heredoc_delim)
	{
		printf("Failed to allocate memory for heredoc delimiter\n");
		return (1);
	}
	//printf("heredoc_delim after copy: %s\n", sh->cmds[index]->heredoc_delim);
	mini->cmds[index]->redirect_type = HEREDOC;
	mini->cmds[index]->heredoc = true;
	return (0);
}

// Handles append redirection, finds the filename and copies data to the redir linked list  

int	handle_append(t_cmd *cmd, int *i)
{
	char	*filename_start;
	int		filename_length;
	
	filename_length = 0;
	(*i)++;
	(*i)++;
	while (cmd->segment[*i]&& ft_isspace(cmd->segment[*i]))
		(*i)++;
	filename_start = &cmd->segment[*i];
	while (cmd->segment[*i] && cmd->segment[*i] != ' ' && cmd->segment[*i] != '|' &&
		cmd->segment[*i]!= '<' && cmd->segment[*i]!= '>' && cmd->segment[*i] != '$')
	{
		filename_length++;
		(*i)++;
	}
	mini->cmds[index]->append = ft_strndup(filename_start, filename_length);
	if (!mini->cmds[index]->append)
	{
		printf("Failed to allocate memory for filename\n");
		return (1);
	}
	mini->cmds[index]->redirect_type = APPEND;
	return (0);
}
