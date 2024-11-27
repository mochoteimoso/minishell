/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:06:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/27 14:09:16 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			check_redirects(char *input);
int			validate_redirect(char *input, int *i, char *type);
static int	check_in_redir(char *input, int *i);
static int	check_out_redir(char *input, int *i);

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
