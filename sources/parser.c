/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/05 17:34:11 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			count_pipes(char *line);
t_command *initialize_cmd_array(int count);
void		add_single_command(t_command *cmd, char *command);
void		add_cmd_to_array(t_command *cmds, int *count, int size, char *command);
char		**split_input_by_pipes(char *line, t_mini *mini);
int			parse_input_segments(t_mini *mini, t_command *cmds);
int			parse_cmd_string(t_mini *mini, t_command *cmds, int index);
char		*ft_strndup(const char *src, size_t n);
bool		is_redirection(char *str, int index);
void		handle_redirect_in(char *str, int *i, t_command *cmds);
//void		handle_redirect_out(char *str, int *i, t_command *cmds);
//void		handle_heredoc(char *str, int *i, t_command *cmds);
//void		handle_append(char *str, int *i, t_command *cmds);

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

int	count_pipes(char *line)
{
	int	i;
	int	pipe_count;

	i = 0;
	pipe_count = 0;
	while (line[i])
	{
		if (line[i] == '|' && !is_in_quotes(line, i))
			pipe_count++;
		i++;
	}
	return (pipe_count);
}

t_command *initialize_cmd_array(int count)
{
	t_command *cmds;

	cmds = ft_calloc(sizeof(t_command), count + 1);
	if (!cmds)
	{
		perror("Failed to allocate memory for struct array");
		exit(EXIT_FAILURE);
	}
	return (cmds);
}

void	add_single_command(t_command *cmd, char *command)
{
	cmd->command = ft_strdup(command);
	cmd->args = NULL;
	cmd->args_count = 0;
	cmd->redirect_in = NULL;
	cmd->redirect_out = NULL;
	cmd->redirect_type = 0;
	cmd->append = false;
	cmd->heredoc = false;
	cmd->heredoc_delim = NULL;
	cmd->env_vars = NULL;
	cmd->env_var_count = 0;
	cmd->exit_status = 0;
}

void	add_cmd_to_array(t_command *cmds, int *count, int size, char *command)
{
	if (*count >= size)
	{
		printf("Array overflow");
	}
	cmds[*count].command = ft_strdup(command);
	cmds[*count].args = NULL;
	cmds[*count].args_count = 0;
	cmds[*count].redirect_in = NULL;
	cmds[*count].redirect_out = NULL;
	cmds[*count].redirect_type = 0;
	cmds[*count].append = false;
	cmds[*count].heredoc = false;
	cmds[*count].heredoc_delim = NULL;
	cmds[*count].env_vars = NULL;
	cmds[*count].env_var_count = 0;
	cmds[*count].exit_status = 0;
	(*count)++;
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
/*
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

// add tokens with content and type to list of tokens

/*void	add_token(t_tokens **tokens, void *content, t_tok_type type)
{
	t_tokens	*new;
	t_tokens	*temp;

	new = malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	new->content = content;
	new->type;
	new->next = NULL;
	if (!(*tokens))
		*tokens = new;
	else
	{
		temp = *tokens;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
} */


/*t_tokens	*add_new_token(void *content, t_tok_type type)
{
	t_tokens	*new;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	new->content = content;
	new->type;
	new->next = NULL;
	return (new);
} */

/*
t_tokens	*tokenizer(char *line)
{
	t_tokens	*tokens;
	int			i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			i++;
		else if (line[i] == '|')
		{
			add_token(&tokens, "|", PIPE);
			i++;
		}
		else if (line[i] == '>')
		{
			
		}
	}
}

void	handle_redirects(char *line, t_tokens **tokens, int i)
{
	if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token(tokens, ">>", APPEND);
			return (i + 2);
		}
		else
		{
			add_token(tokens, ">", REDIRECT_OUT);
			return (i + 1);
		}
	}
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token(tokens, "<<", HEREDOC);
			return (i + 2);
		}
		else
		{
			add_token(tokens, "<", REDIRECT_IN);
			return (i + 1);
		}
	}
	return (i);
} */
