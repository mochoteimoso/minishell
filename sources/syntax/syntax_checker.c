/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:45:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/03 09:52:19 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		validate_input_syntax(char **input);
int		check_quotes(char *input, int limit);
int		check_non_whitespace(char *str);

/* Checks the syntax of the input string
for unmatched quotes and incorrectly placed pipes and redirection symbols*/

int	validate_input_syntax(char **input)
{
	int	i;

	if (check_quotes(*input, 0))
	{
		ft_putendl_fd("syntax error: unmatched quotes", 2);
		return (1);
	}
	if (check_pipes(input))
		return (1);
	i = 0;
	while ((*input)[i])
	{
		if (!check_quotes(*input, i) &&
			((*input)[i] == ';' || (*input)[i] == '\\'))
		{
			ft_putendl_fd("invalid syntax", 2);
			return (1);
		}
		i++;
	}
	if (check_redirects(*input))
		return (1);
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

/* Checks whether a string contains non-whitespace characters */

int	check_non_whitespace(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (1);
		str++;
	}
	return (0);
}

