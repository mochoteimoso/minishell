/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:05:32 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/09 16:55:31 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_redirection(char *str, int index);
int		handle_redirect_in(char *str, int *i, t_shell *sh, int index);
int		handle_redirect_out(char *str, int *i, t_shell *sh, int index);
int		handle_heredoc(char *str, int *i, t_shell *sh, int index);
int		handle_append(char *str, int *i, t_shell *sh, int index);

// Checks if the input contains a redirection symbol that is not within quotes

bool	is_redirection(char *str, int i)
{
	if ((str[i] == '>' || str[i] == '<') && !is_in_quotes(str, i))
		return (true);
	else
		return (false);
}

// Handles < redirection, finds the filename and copies data to the redir linked list

int	handle_redirect_in(char *str, int *i, t_shell *sh, int index)
{
	char	*filename_start;
	int		filename_length;

	filename_length = 0;
	(*i)++;
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
	filename_start = &str[*i];
	while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' &&
		str[*i] != '>' && str[*i] != '$')
	{
		filename_length++;
		(*i)++;
	}
	sh->cmds[index]->redirect_in = ft_strndup(filename_start, filename_length);
	if (!sh->cmds[index]->redirect_in)
	{
		printf("Failed to allocate memory for filename\n");
		return (1);
	}
	sh->cmds[index]->redirect_type = REDIRECT_IN;
	return (0);
}

// Handles > redirection, finds the filename and copies data to the redir linked list

int	handle_redirect_out(char *str, int *i, t_shell *sh, int index)
{
	char	*filename_start;
	int		filename_length;

	filename_length = 0;
	(*i)++;
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
	filename_start = &str[*i];
	while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' &&
		str[*i] != '>' && str[*i] != '$')
	{
		filename_length++;
		(*i)++;
	}
	sh->cmds[index]->redirect_out = ft_strndup(filename_start, filename_length);
	if (!sh->cmds[index]->redirect_out)
	{
		printf("Failed to allocate memory for filename\n");
		return (1);
	}
	sh->cmds[index]->redirect_type = REDIRECT_OUT;
	return (0);
}

// Handles heredoc, finds the delimiter and copies data to the redir linked list

int	handle_heredoc(char *str, int *i, t_shell *sh, int index)
{
	char	*delimiter_start;
	int		delimiter_length;

	delimiter_length = 0;
	(*i)++;
	(*i)++;
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
	delimiter_start = &str[*i];
	while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' &&
		str[*i] != '>' && str[*i] != '$')
	{
		delimiter_length++;
		(*i)++;
	}
	sh->cmds[index]->heredoc_delim = ft_strndup(delimiter_start, delimiter_length);
	if (!sh->cmds[index]->heredoc_delim)
	{
		printf("Failed to allocate memory for heredoc delimiter\n");
		return (1);
	}
	//printf("heredoc_delim after copy: %s\n", sh->cmds[index]->heredoc_delim);
	sh->cmds[index]->redirect_type = HEREDOC;
	sh->cmds[index]->heredoc = true;
	return (0);
}

// Handles append redirection, finds the filename and copies data to the redir linked list

int	handle_append(char *str, int *i, t_shell *sh, int index)
{
	char	*filename_start;
	int		filename_length;

	filename_length = 0;
	(*i)++;
	(*i)++;
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
	filename_start = &str[*i];
	while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' &&
		str[*i] != '>' && str[*i] != '$')
	{
		filename_length++;
		(*i)++;
	}
	sh->cmds[index]->append = ft_strndup(filename_start, filename_length);
	if (!sh->cmds[index]->append)
	{
		printf("Failed to allocate memory for filename\n");
		return (1);
	}
	sh->cmds[index]->redirect_type = APPEND;
	return (0);
}
