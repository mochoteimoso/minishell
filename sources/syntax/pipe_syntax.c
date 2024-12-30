/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:06:20 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/30 15:39:23 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			check_pipes(char **input, t_shell *mini);
static int	check_consecutive_pipes(char *input, t_shell *mini);
static int	check_pipe_error(char *input, int i, t_shell *mini);
static int	check_trailing_pipe(char **input);

/* Checks syntax for pipes, i.e. that it is not a the start or that
there are no consecutive pipes. Also handles a trailing pipe */

int	check_pipes(char **input, t_shell *mini)
{
	int	i;

	i = 0;
	while ((*input)[i] == ' ')
		i++;
	if ((*input)[i] == '|' && !check_quotes(*input, i))
	{
		ft_putendl_fd("syntax error near unexpected token ", 2);
		mini->exit_stat = 2;
		return (1);
	}
	if (check_consecutive_pipes(*input, mini))
		return (1);
	if (check_trailing_pipe(input))
		return (1);
	return (0);
}

// checks if there are consecutive pipes without text in between

static int	check_consecutive_pipes(char *input, t_shell *mini)
{
	int	i;
	int	pipe_found;

	i = 0;
	pipe_found = 0;
	while (input[i])
	{
		if (input[i] == '|' && !check_quotes(input, i))
		{
			if (check_pipe_error(input, i, mini))
				return (1);
			pipe_found = 1;
		}
		else if (input[i] == ' ')
			pipe_found = 0;
		i++;
	}
	return (0);
}

static int	check_pipe_error(char *input, int i, t_shell *mini)
{
	int	j;

	j = i + 1;
	while (input[j] == ' ')
		j++;
	if (input[j] == '|' && !check_quotes(input, j))
	{
		ft_putstr_fd("syntax error near unexpected token ", 2);
		ft_putendl_fd(&input[i], 2);
		mini->exit_stat = 2;
		return (1);
	}
	return (0);
}

// Checks for trailing pipe

static int	check_trailing_pipe(char **input)
{
	int		i;
	char	*updated_input;

	i = ft_strlen(*input) - 1;
	while (i >= 0 && (*input)[i] == ' ')
		i--;
	if (i >= 0 && (*input)[i] == '|' && !check_quotes(*input, i))
	{
		updated_input = handle_trailing_pipe(*input);
		if (!updated_input)
		{
			*input = NULL;
			return (1);
		}
		*input = updated_input;
	}
	return (0);
}
