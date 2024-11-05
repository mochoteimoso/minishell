/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/05 19:16:00 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**split_input_by_pipes(char *line, t_mini *mini);
int		parse_input_segments(t_mini *mini, t_command *cmds);
int		parse_cmd_string(t_mini *mini, t_command *cmds, int index);
char	*ft_strndup(const char *src, size_t n);
bool	is_redirection(char *str, int index);
void	handle_redirect_in(char *str, int *i, t_command *cmds);
//void	handle_redirect_out(char *str, int *i, t_command *cmds);
//void	handle_heredoc(char *str, int *i, t_command *cmds);
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

char	**split_input_by_pipes(char *line, t_mini *mini)
{
	int		i;
	char	delimiter;

	i = 0;
	delimiter = '|';
	if (!line)
		return (NULL);
	mini->split_input = ft_split(line, delimiter);
	if (!mini->split_input)
	{
		printf("failed to split input string");
		return (NULL);
	}
	return (mini->split_input);
}

int	parse_input_segments(t_mini *mini, t_command *cmds)
{
	int	i;

	i = 0;
	while (mini->split_input[i])
	{
		if (parse_cmd_string(mini, cmds, i))
			return (1);
		i++;
	}
	return (0);
}

int	parse_cmd_string(t_mini *mini, t_command *cmds, int index)
{
	int		i;
	char	*cmd_string;

	i = 0;
	cmd_string = mini->split_input[index];
	while (cmd_string[i])
	{
		if (is_redirection(cmd_string, i))
		{
			//if (cmd_string[i] == '<' && cmd_string[i + 1] == '<')
				//handle_heredoc(cmd_string, &i, cmds);
			//else if (cmd_string[i] == '>' && cmd_string[i + 1] == '>')
			//	handle_append(cmd_string, &i, cmds);
			if (cmd_string[i] == '<')
				handle_redirect_in(cmd_string, &i, cmds);
			//else if (cmd_string[i] == '>')
			//	handle_redirect_out(cmd_string, &i, cmds);
		}
		//else
		//{
		//	tokenize_command(&cmds[index], cmd_string + i);
		//	i++;
		//}
	}
	return (0);
}

bool	is_redirection(char *str, int index)
{
	if ((str[index] == '>' || str[index] == '<') && !is_in_quotes(str, index))
		return (true);
	else
		return (false);
}

void	handle_redirect_in(char *str, int *i, t_command *cmds)
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
/*
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
}

void	handle_heredoc(char *str, int *i, t_command *cmds)
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
	cmds->heredoc_delim = ft_strndup(delimiter_start, delimiter_length);
	//add error handling
	cmds->redirect_type = HEREDOC;
	cmds->heredoc = true;
}

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