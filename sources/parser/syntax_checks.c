/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:45:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/27 16:21:01 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_input_syntax(char *input);
int	check_consecutive_pipes(char *input);
int	check_pipes(char *input);
int	check_redirects(char *input);
int	validate_redirect(char *input, int *i, char *type);

/* Checks the syntax of the input string
for unmatched quotes and incorrectly placed pipes and redirects*/

int	validate_input_syntax(char *input)
{
	if (check_quotes(input, -1))
	{
		ft_putendl_fd("syntax error: unmatched quotes", 2);
		return (1);
	}
	if (check_pipes(input))
		return (1);
	if (*input == ';' || *input == '\\')
	{
		printf("invalid syntax");
		return (1);
	}
	if (check_redirects(input))
		return (1);
	else
		return (0);
}

/* Checks if a given index is within quotes or if there are unbalanced quotes in the string*/
int check_quotes(char *input, int limit)
{
	int in_single_quote;
	int in_double_quote;
	int index;

	in_single_quote = 0;
	in_double_quote = 0;
	index = 0;
	while (input[index] && (limit == -1 || index <= limit))
	{
		if (input[index] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[index] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		index++;
	}
	if (limit == -1)
	{
		return (in_single_quote || in_double_quote);
	}
	return (in_single_quote || in_double_quote);
}

//Checks whether pipe is at an invalid location, i.e. at the start or end of input
int	check_pipes(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ')
		i++;
	if (input[i] == '|' && !check_quotes(input, -1))
	{
		printf("syntax error near unexpected token %c\n", input[i]);
		return (1);
	}
	if (check_consecutive_pipes(input))
	{
		printf("syntax error near unexpected token %c\n", input[i]);
		return (1);
	}
	while (input[i])
		i++;
	while (i >= 0 && input[i - 1] == ' ')
		i--;
	if (input[i] == '|' && !check_quotes(input, -1))
	{
		printf("syntax error near unexpected token %c\n", input[i]);
		return (1);
	}
	return (0);
}

// checks if there are consecutive pipes without text in between
int	check_consecutive_pipes(char *input)
{
	int	i;
	int	pipe_found;

	i = 0;
	pipe_found = 0;
	while (input[i])
	{
		if (input[i] == '|' && !check_quotes(input, i))
		{
			if (pipe_found)
			{
				printf("syntax error near unexpected token %c\n", input[i]);
				return (1);
			}
			pipe_found = 1;
		}
		else if (input[i] == ' ')
			pipe_found = 0;
		i++;
	}
	return (0);
}

static int	check_in_redir(char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		(*i)++;
		if (validate_redirect(input, i, ">>") != 0)
			return (1);
	}
	else
	{
		if (validate_redirect(input, i, ">") != 0)
			return (1);
	}
	return (0);
}

static int	check_out_redir(char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		(*i)++;
		if (validate_redirect(input, i, "<<") != 0)
			return (1);
	}
	else
	{
		if (validate_redirect(input, i, "<") != 0)
			return (1);
	}
	return (0);
}

// Checks that there is a non-space character after redirects
int	check_redirects(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' && !check_quotes(input, i))
		{
			if (check_in_redir(input, &i))
				return (1);
		}
		else if (input[i] == '<' && !check_quotes(input, i))
		{
			if (check_out_redir(input, &i))
				return (1);
		}
		i++;
	}
	return (0);
}

// checks that there is a non-space character after redirect
int	validate_redirect(char *input, int *i, char *type)
{
	(*i)++;
	while (input[*i] == ' ')
		(*i)++;
	if (!input[*i] || input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		printf("syntax error near unexpected token %s\n", type);
		return (1);
	}
	return (0);
}

