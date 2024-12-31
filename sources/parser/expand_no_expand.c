/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_no_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:29:31 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 17:12:26 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			no_expanding(t_shell *mini, char *str, t_expand *arg);
static int	this_breaks(t_expand *arg, char *str);
static int	no_expand_dollar(t_expand *arg, char *str);

/**
 * no_expanding - Handles parsing of segments where
 * 				  variable expansion is not allowed.
 *
 * @mini: Pointer to the shell structure for context.
 * @str: Command string to process.
 * @arg: Pointer to the t_expand structure tracking parsing state.
 *
 * Parses segments without expanding variables,
 * handling special cases such as heredoc.
 * Updates the t_expand structure with the parsed segment.
 * Returns the updated index or -1 on failure.
 */
int	no_expanding(t_shell *mini, char *str, t_expand *arg)
{
	arg->sgl = 0;
	arg->dbl = 0;
	arg->start = arg->i;
	what_quote(str, arg);
	while (str[arg->i])
	{
		if (this_breaks(arg, str) == 1)
			break ;
		if (no_expand_dollar(arg, str) == 1)
			arg->i++;
		if (no_expand_dollar(arg, str) == 2)
		{
			if (we_have_dollar(mini, arg, str))
				return (-1);
		}
		else if (add_char(str, arg))
			return (-1);
		if (str[arg->i] == '<' && str[arg->i + 1] == '<')
		{
			arg->i = we_have_heredoc(arg, str, 1);
			if (arg->i == -1)
				return (-1);
		}
	}
	return (arg->i);
}

/**
 * this_breaks - Determines if a character breaks the current segment.
 *
 * @arg: Pointer to the t_expand structure tracking parsing state.
 * @str: Command string to check.
 *
 * Checks if the current character is a space, single quote, or double quote,
 * which would indicate the end of the current segment.
 * Returns 1 if the character breaks the segment, 0 otherwise.
 */
static int	this_breaks(t_expand *arg, char *str)
{
	if (ft_isspace(str[arg->i]) || str[arg->i] == '\'' || str[arg->i] == '"')
		return (1);
	else
		return (0);
}

/**
 * no_expand_dollar - Determines the type of dollar ('$') usage in the segment.
 *
 * @arg: Pointer to the t_expand structure tracking parsing state.
 * @str: Command string containing the segment.
 *
 * Analyzes the dollar sign ('$') to determine if it should be ignored, expanded,
 * or treated as part of a quoted segment. Handles cases for single quotes,
 * double quotes, and unquoted variables.
 * Returns 1 to ignore, 2 to expand, or 0 for normal processing.
 */
static int	no_expand_dollar(t_expand *arg, char *str)
{
	if (str[arg->i] == '$' && (str[arg->i + 1] == '"'
			|| str[arg->i + 1] == '\'') && !arg->dbl && !arg->sgl)
		return (1);
	if ((((arg->dbl && !arg->sgl) || (!arg->dbl && !arg->sgl))
			&& str[arg->i] == '$' && ((str[arg->i + 1]
					&& ft_isalnum(str[arg->i + 1]))
				|| str[arg->i + 1] == '?')))
		return (2);
	return (0);
}
