/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:45:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/09 16:30:56 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_input_syntax(char *input);
int	is_in_quotes(char *input, int i);
int	has_quotes(char *input, int i);
int	check_consecutive_pipes(char *input);
int	check_pipes(char *input);
int	check_redirects(char *input);
int	validate_redirect(char *input, int *i, char *type);

// Checks the syntax of the input string
// for unmatched quotes and incorrectly placed pipes and redirects

int	validate_input_syntax(char *input)
{
	if (!input)
		return (1);
	if (has_quotes(input, 0))
	{
		printf("syntax error: unmatched quotes");
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

// Checks whether special character is in quotes or not

int	is_in_quotes(char *input, int i)
{
	int	in_single_quote;
	int	in_double_quote;
	int	index;

	in_single_quote = 0;
	in_double_quote = 0;
	index = 0;
	while (index <= i && input[index])
	{
		if (input[index] == '\'')
			in_single_quote = !in_single_quote;
		else if (input[index] == '"')
			in_double_quote = !in_double_quote;
		index++;
	}
	if (in_double_quote || in_single_quote)
		return (1);
	else
		return (0);
}

//Checks whether pipe is at an invalid location, i.e. at the start or end of input

int	check_pipes(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ')
		i++;

	// check that pipe is not first non-space character

	if (input[i] == '|' && !is_in_quotes(input, i))
	{
		printf("syntax error near unexpected token %c\n", input[i]);
		return (1);
	}

	// check that there are no consecutive pipes

	if (check_consecutive_pipes(input))
	{
		printf("syntax error near unexpected token %c\n", input[i]);
		return (1);
	}

	//check that the pipe is not the last non-space character

	while (input[i])
		i++;
	while (i >= 0 && input[i - 1] == ' ')
		i--;
	if (input[i - 1] == '|' && !is_in_quotes(input, i - 1))
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
		if (input[i] == '|' && !is_in_quotes(input, i))
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

// Checks that there is a non-space character after redirects

int	check_redirects(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' && !is_in_quotes(input, i))
		{
			if (input[i + 1] == '>')
			{
				i++;
				if (validate_redirect(input, &i, ">>") != 0)
					return (1);
			}
			else
			{
				if (validate_redirect(input, &i, ">") != 0)
					return (1);
			}
		}
		else if (input[i] == '<' && !is_in_quotes(input, i))
		{
			if (input[i + 1] == '<')
			{
				i++;
				if (validate_redirect(input, &i, "<<") != 0)
					return (1);
			}
			else
			{
				if (validate_redirect(input, &i, "<") != 0)
					return (1);
			}
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


// This needs to be combined with is_in_quotes function - figure out how

int	has_quotes(char *input, int i)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '"' && in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	if (in_double_quote || in_single_quote)
		return (1);
	return (0);
}
