/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/07 14:07:17 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		split_input_by_pipes(char *line, t_shell *sh);
int		parse_input_segments(t_shell *sh);
int		parse_cmd_string(t_shell *sh, int index);
char	*ft_strndup(const char *src, size_t n);
bool	is_redirection(char *str, int index);
//void	handle_redirect_in(char *str, int *i, t_command *cmds);
//void	handle_redirect_out(char *str, int *i, t_command *cmds);
void	handle_heredoc(char *str, int *i, t_shell *sh, int index);
//void	handle_append(char *str, int *i, t_command *cmds);

char	*ft_strndup(const char *src, size_t n)
{
	size_t	src_len;
	size_t	size;
	size_t	i;
	char	*dest;

	src_len = ft_strlen(src);
	if (src_len < n)
		size = src_len;
	else
		size = n;
	dest = (char *) malloc(size * sizeof(char) + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[size] = '\0';
	return (dest);
}

/*int	split_input_by_pipes(char *line, t_shell *sh)
{
	int		i;
	char	delimiter;

	i = 0;
	delimiter = '|';
	if (!line)
		return (1);
	sh->split_input = ft_split(line, delimiter);
	if (!sh->split_input)
	{
		printf("Failed to split input string");
		return (1);
	}
	printf("Split input by pipes:\n");
	while (sh->split_input[i])
	{
		printf("Segment %d: %s\n", i, sh->split_input[i]);
		i++;
	}
	return (0);
} */

int	parse_input_segments(t_shell *sh)
{
	int	index;

	index = 0;
	while (sh->cmds[index])
	{
		if (parse_cmd_string(sh, index))
			return (1);
		index++;
	}
	return (0);
}

int	parse_cmd_string(t_shell *sh, int index)
{
	int		i;
	char	*cmd_string;

	i = 0;
	cmd_string = sh->cmds[index]->segment;
	while (cmd_string[i])
	{
		if (is_redirection(cmd_string, i))
		{
			if (cmd_string[i] == '<' && cmd_string[i + 1] == '<')
				handle_heredoc(cmd_string, &i, sh, index);
			//else if (cmd_string[i] == '>' && cmd_string[i + 1] == '>')
			//	handle_append(cmd_string, &i, cmds);
			//if (cmd_string[i] == '<')
				//handle_redirect_in(cmd_string, &i, cmds);
			//else if (cmd_string[i] == '>')
			//	handle_redirect_out(cmd_string, &i, cmds);
		}
		//else
		//{
		//	tokenize_command(&cmds[index], cmd_string + i);
		//	i++;
		//}
		i++;
	}
	return (0);
}

bool	is_redirection(char *str, int i)
{
	if ((str[i] == '>' || str[i] == '<') && !is_in_quotes(str, i))
		return (true);
	else
		return (false);
}

/*void	handle_redirect_in(char *str, int *i, t_command *cmds)
{
	char	*filename_start;
	int		filename_length;
	
	filename_length = 0;
	(*i)++;
	while (str[*i] == ' ')
		(*i)++;
	filename_start = &str[*i];
	while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' &&
		str[*i] != '>' && str[*i] != '$')
	{
		filename_length++;
		(*i)++;
	}
	cmds->redirect_in = ft_strndup(filename_start, filename_length);
	//add error handling
	cmds-> redirect_type = REDIRECT_IN;
}

void	handle_redirect_out(char *str, int *i, t_command *cmds)
{
	char	*filename_start;
	int		filename_length;
	
	filename_length = 0;
	(*i)++;
	while (str[*i] == ' ')
		(*i)++;
	filename_start = &str[*i];
	while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' &&
		str[*i] != '>' && str[*i] != '$')
	{
		filename_length++;
		(*i)++;
	}
	cmds->redirect_out = ft_strndup(filename_start, filename_length);
	//add error handling
	cmds-> redirect_type = REDIRECT_OUT;
} */

void	handle_heredoc(char *str, int *i, t_shell *sh, int index)
{
	char	*delimiter_start;
	int		delimiter_length;
	
	delimiter_length = 0;
	(*i)++;
	(*i)++;
	while (str[*i] == ' ')
		(*i)++;
	delimiter_start = &str[*i];
	while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' &&
		str[*i] != '>' && str[*i] != '$')
	{
		delimiter_length++;
		(*i)++;
	}
	sh->cmds[index]->heredoc_delim = ft_strndup(delimiter_start, delimiter_length);
	//add error handling
	sh->cmds[index]->redirect_type = HEREDOC;
	sh->cmds[index]->heredoc = true;
}
/*
void	handle_append(char *str, int *i, t_command *cmds)
{
	char	*filename_start;
	int		filename_length;
	
	filename_length = 0;
	(*i)++;
	(*i)++;
	while (str[*i] == ' ')
		(*i)++;
	filename_start = &str[*i];
	while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' &&
		str[*i] != '>' && str[*i] != '$')
	{
		filename_length++;
		(*i)++;
	}
	cmds->append = ft_strndup(filename_start, filename_length);
	//add error handling
	cmds-> redirect_type = APPEND;
} */