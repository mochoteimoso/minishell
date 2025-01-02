/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quoted.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:12:11 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:54:06 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			in_quotes(t_shell *mini, char *str, int i, t_expand *arg);
int			we_have_heredoc(t_expand *arg, char *str, int n);
static int	init_quoted(t_expand *arg, int i, char *str);
static int	handle_dollar(t_shell *mini, t_expand *arg, char *str);
static int	handle_quotes(t_expand *arg, char *str);

/**
 * in_quotes - Processes a quoted segment of the command.
 *
 * @mini: Pointer to the shell structure for context.
 * @str: Command string containing the quoted segment.
 * @i: Current index in the command string.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 *
 * Handles parsing and expanding variables within single or double quotes.
 * Returns the updated index after processing or -1 on failure.
 */
int	in_quotes(t_shell *mini, char *str, int i, t_expand *arg)
{
	if (init_quoted(arg, i, str) == -1)
		return (-1);
	what_quote(str, arg);
	while (str[arg->i])
	{
		if (!arg->sgl && !arg->dbl)
			break ;
		if (arg->i != -1 && str[arg->i] == '$')
			arg->i = handle_dollar(mini, arg, str);
		if (arg->i != -1 && ((!arg->sgl && !arg->dbl && (str[arg->i] == '\''
						|| str[arg->i] == '"'))
				|| ((arg->sgl && str[arg->i] == '\'')
					|| (arg->dbl && str[arg->i] == '"'))))
			arg->i = handle_quotes(arg, str);
		else if (arg->i != -1)
		{
			if (add_char(str, arg))
				return (-1);
		}
		if (arg->i == -1)
			return (-1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

/**
 * we_have_heredoc - Parses a heredoc segment
 * 					 and stores it in the expansion structure.
 *
 * @arg: Pointer to the t_expand structure tracking heredoc parsing state.
 * @str: Command string containing the heredoc segment.
 * @n: Flag indicating whether to initialize the value.
 *
 * Extracts the heredoc string from the input and updates the t_expand structure.
 * Returns the updated index or -1 on failure.
 */
int	we_have_heredoc(t_expand *arg, char *str, int n)
{
	if (!n)
		arg->value = ft_strdup("");
	while (str[arg->i] == '<' || str[arg->i] == ' ')
	{
		if (add_char(str, arg))
			return (-1);
	}
	while (str[arg->i] && (!ft_isspace(str[arg->i])
			|| !(str[arg->i] == '<') || !(str[arg->i] == '>')))
	{
		if (add_char(str, arg))
			return (-1);
	}
	return (arg->i);
}

/**
 * init_quoted - Initializes a quoted segment for processing.
 *
 * @arg: Pointer to the t_expand structure tracking expansion state.
 * @i: Current index in the command string.
 * @str: Command string containing the quoted segment.
 *
 * Prepares the t_expand structure for processing a quoted segment by
 * including the initial quote character.
 * Returns the updated index or -1 on failure.
 */
static int	init_quoted(t_expand *arg, int i, char *str)
{
	char	*temp;

	if (the_arg(arg, i))
		return (-1);
	temp = arg->value;
	arg->value = ft_strjoin_char(temp, str[arg->i]);
	free(temp);
	if (!arg->value)
		return (-1);
	return (arg->i);
}

/**
 * handle_dollar - Processes dollar sign ('$') expansion within a quoted segment.
 *
 * @mini: Pointer to the shell structure for context.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 * @str: Command string containing the segment.
 *
 * Handles expansion of variables denoted by '$' in the quoted segment,
 * excluding single-quoted sections. Returns the updated index or -1 on failure.
 */
static int	handle_dollar(t_shell *mini, t_expand *arg, char *str)
{
	if (str[arg->i] == '$' && str[arg->i + 1]
		&& (str[arg->i + 1] == '\'' || str[arg->i + 1] == '"')
		&& (!arg->sgl && !arg->dbl))
		arg->i++;
	if ((str[arg->i] == '$' && !arg->sgl) && (str[arg->i + 1]
			&& !ft_isspace(str[arg->i + 1]) && (ft_isalnum(str[arg->i + 1])
				|| str[arg->i + 1] == '_' || str[arg->i + 1] == '?')))
	{
		if (we_have_dollar(mini, arg, str) == -1)
			return (-1);
	}
	return (arg->i);
}

/**
 * handle_quotes - Processes quote transitions within a segment.
 *
 * @arg: Pointer to the t_expand structure tracking expansion state.
 * @str: Command string containing the segment.
 *
 * Updates the state of the t_expand structure when encountering opening or
 * closing single or double quotes, appending the quote character to the value.
 * Returns the updated index or -1 on failure.
 */
static int	handle_quotes(t_expand *arg, char *str)
{
	char	*temp;

	if (!arg->sgl && !arg->dbl && (str[arg->i] == '\''
			|| str[arg->i] == '"'))
		what_quote(str, arg);
	else if ((arg->sgl && str[arg->i] == '\'')
		|| (arg->dbl && str[arg->i] == '"'))
	{
		temp = arg->value;
		arg->value = ft_strjoin_char(temp, str[arg->i]);
		what_quote(str, arg);
		free(temp);
	}
	return (arg->i);
}
