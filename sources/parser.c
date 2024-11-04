/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/04 19:11:46 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			count_pipes(char *line);
t_command *initialize_cmd_array(int count);
void		add_single_command(t_command *cmd, char *command);
void		add_cmd_to_array(t_command *cmds, int *count, int size, char *command);

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
	(*count)++;
}


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
