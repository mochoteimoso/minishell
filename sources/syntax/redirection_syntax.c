/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:06:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 14:21:38 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			check_redirects(char *input, t_shell *mini);
static int	check_in_redir(char *input, t_shell *mini, int *i);
static int	check_out_redir(char *input, t_shell *mini, int *i);
static int	validate_redirect(char *input, t_shell *mini, int *i, char *type);

// Checks that there is a non-whitespace character after redirects

int	check_redirects(char *input, t_shell *mini)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' && !check_quotes(input, i))
		{
			if (check_in_redir(input, mini, &i))
				return (1);
		}
		else if (input[i] == '<' && !check_quotes(input, i))
		{
			if (check_out_redir(input, mini, &i))
				return (1);
		}
		i++;
	}
	return (0);
}

static int	check_in_redir(char *input, t_shell *mini, int *i)
{
	if (input[*i + 1] == '>')
	{
		(*i)++;
		if (validate_redirect(input, mini, i, ">>") != 0)
			return (1);
	}
	else
	{
		if (validate_redirect(input, mini, i, ">") != 0)
			return (1);
	}
	return (0);
}

static int	check_out_redir(char *input, t_shell *mini, int *i)
{
	if (input[*i + 1] == '<')
	{
		(*i)++;
		if (validate_redirect(input, mini, i, "<<") != 0)
			return (1);
	}
	else
	{
		if (validate_redirect(input, mini, i, "<") != 0)
			return (1);
	}
	return (0);
}

// Returns an error if there is no non-whitespace character
// after redirection symbol before input ends or a pipe is encountered

static int	validate_redirect(char *input, t_shell *mini, int *i, char *type)
{
	(*i)++;
	while (input[*i] == ' ')
		(*i)++;
	if (!input[*i] || input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		ft_putstr_fd("syntax error near unexpected token ", 2);
		ft_putendl_fd(type, 2);
		mini->exit_stat = 2;
		return (1);
	}
	return (0);
}
