/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:45:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/27 15:35:15 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_input_syntax(char *input);
int	check_consecutive_pipes(char *input);
int	check_pipes(char *input);
int	check_redirects(char *input);

/* Checks the syntax of the input string
for unmatched quotes and incorrectly placed pipes and redirection symbols*/

int	validate_input_syntax(char *input)
{
	if (check_quotes(input, 0))
	{
		ft_putendl_fd("syntax error: unmatched quotes", 2);
		return (1);
	}
	if (check_pipes(input))
		return (1);
	if (*input == ';' || *input == '\\')
	{
		ft_putendl_fd("invalid syntax", 2);
		return (1);
	}
	if (check_redirects(input))
		return (1);
	else
		return (0);
}

/* Checks if a given index is within quotes or if there are unbalanced quotes in the string*/

int	check_quotes(char *input, int limit)
{
	int	in_single_quote;
	int	in_double_quote;
	int	index;

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
	if (input[i] == '|' && !check_quotes(input, i))
	{
		printf("syntax error near unexpected token %c\n", input[i]);
		return (1);
	}
	if (check_consecutive_pipes(input))
		return (1);
	while (input[i])
		i++;
	if (check_trailing_pipe(input))
		return (1);
	while (i >= 0 && input[i - 1] == ' ')
		i--;
	if (input[i - 1] == '|' && !check_quotes(input, i - 1))
	{
		printf("syntax error near unexpected token '%c'\n", input[i]);
		return (1);
	}
	return (0);
}

// checks if there are consecutive pipes without text in between

int	check_consecutive_pipes(char *input)
{
	int	i;
	int	j;
	int	pipe_found;

	i = 0;
	pipe_found = 0;
	while (input[i])
	{
		if (input[i] == '|' && !check_quotes(input, i))
		{
			j = i + 1;
			while (input[j] == ' ')
				j++;
			if (input[j] == '|' && !check_quotes(input, j))
			{
				printf("syntax error near unexpected token '%c'\n", input[i]);
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

int	check_trailing_pipe(char *input)
{
	int	i;

	i = ft_strlen(input) - 1;
	while (i >= 0 && input[i]== ' ')
		i--;
	if (i >= 0 && input[i] == '|' && !check_quotes(input, i))
	{
		if (handle_trailing_pipe(input))
			return (0);
		else
		{
			ft_putendl_fd("syntax error: unexpected end of input\n", 2);
			return (1);
		}
	}
	return (0);
}

int	handle_trailing_pipe(char *input)
{
	char *additional_input;
	char *new_input;

	additional_input = NULL;
	new_input = NULL;
	while (1)
	{
		additional_input = readline(">");
		if (!additional_input)
			free(additional_input);
		if (check_non_whitespace(additional_input))
		{
			new_input = ft_strjoin(input, additional_input);
			free(additional_input);
			if (!new_input)
			{
				perror("malloc");
				return (0);
			}
			ft_strcpy(input, new_input);
			free(new_input);
			return (1);
		}
		free(additional_input);
	}
	return (0);
}

int check_non_whitespace(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (1);
		str++;
	}
	return (0);
}