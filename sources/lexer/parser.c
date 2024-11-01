/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/10/31 18:00:09 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

// linked list for tokens

typedef struct s_tokens
{
	t_tok_type		type;
	char			*content;
	struct s_tokens	*next;
}	t_tokens;

// enum of token types

typedef enum e_tok_type
{
	WORD,
	PIPE,
	APPEND,
	REDIRECT_IN,    // redirection '<'
	REDIRECT_OUT,   // redirection '>'
	ENV_VAR,
	HEREDOC,
	DELIMITER
} t_tok_type;


// add tokens with content and type to list of tokens

void	add_token(t_tokens **tokens, void *content, t_tok_type type)
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
}


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
}
