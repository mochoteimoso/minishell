/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:06:20 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/04 11:05:14 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			check_pipes(char **input);
static int	validate_pipe(char *input);
static int	check_consecutive_pipes(char *input);
static int	check_trailing_pipe(char **input);
static char	*handle_trailing_pipe(char *input);

/* Checks syntax for pipes, i.e. that it is not a the start or that
there are no consecutive pipes. Also handles a trailing pipe */

int	check_pipes(char **input)
{
	int	i;

	i = 0;
	while ((*input)[i] == ' ')
		i++;
	if ((*input)[i] == '|' && !check_quotes(*input, i))
	{
		printf("syntax error near unexpected token %c\n", (*input)[i]);
		return (1);
	}
	if (validate_pipe(*input))
		return (1);
	if (check_consecutive_pipes(*input))
		return (1);
	if (check_trailing_pipe(input))
		return (1);
	return (0);
}

// Checks that pipe is not followed by '>' without any text in between 

static int	validate_pipe(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|' && !check_quotes(input, i))
		{
			i++;
			while (input[i] == ' ')
				i++;
			if (input[i] == '>')
			{
				ft_putendl_fd("Syntax error: expected a command", 2);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

// checks if there are consecutive pipes without text in between

static int	check_consecutive_pipes(char *input)
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

// Checks for trailing pipe

static int	check_trailing_pipe(char **input)
{
	int		i;
	char	*updated_input;

	i = ft_strlen(*input) - 1;
	while (i >= 0 && (*input)[i]== ' ')
		i--;
	if (i >= 0 && (*input)[i] == '|' && !check_quotes(*input, i))
	{
		updated_input = handle_trailing_pipe(*input);
		if (!updated_input)
		{
			ft_putendl_fd("syntax error: unexpected end of input\n", 2);
			return (1);
		}
		*input = updated_input;
	}
	return (0);
}

/* Handles trailing pipe by getting additional input from user
and joining that to the original input string */

static char	*handle_trailing_pipe(char *input)
{
	char	*additional_input;
	char	*updated_input;

	additional_input = NULL;
	updated_input = NULL;
	while (1)
	{
		additional_input = readline(">");
		if (!additional_input)
		{
			perror("readline error");
			return (NULL);
		}
		if (check_non_whitespace(additional_input))
		{
			updated_input = ft_strjoin(input, additional_input);
			free(additional_input);
			if (!updated_input)
			{
				perror("malloc");
				return (NULL);
			}
			free(input);
			return (updated_input);
		}
		free(additional_input);
	}
}
