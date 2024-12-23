/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_no_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:29:31 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/23 18:17:19 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			no_expanding(t_shell *mini, char *str, t_expand *arg);
static int	this_breaks(t_expand *arg, char *str);
static int	no_expand_dollar(t_expand *arg, char *str);
static int	no_expand_quotes(t_expand *arg, char *str);

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
		else if (str[arg->i] == '\'' || str[arg->i] == '"')
			arg->i = no_expand_quotes(arg, str);
		if (no_expand_dollar(arg, str) == 2)
			we_have_dollar(mini, arg, str);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
		if (str[arg->i] == '<' && str[arg->i + 1] == '<')
		{
			arg->i = we_have_heredoc(arg, str, 1);
			if (str[arg->i] == '\0')
				break ;
		}
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

static int	this_breaks(t_expand *arg, char *str)
{
	if (((str[arg->i] == ' ' || str[arg->i] == '\t')
			|| (str[arg->i] == '\'' || str[arg->i] == '"'))
		|| arg->dbl || arg->sgl)
		return (1);
	else
		return (0);
}

static int	no_expand_quotes(t_expand *arg, char *str)
{
	if (!arg->sgl && !arg->dbl && (str[arg->i] == '\'' || str[arg->i] == '"'))
	{
		arg->value = ft_strjoin_char(arg->value, str[arg->i]);
		what_quote(str, arg);
	}
	else if ((arg->sgl && str[arg->i] == '\'')
		|| (arg->dbl && str[arg->i] == '"'))
	{
		arg->value = ft_strjoin_char(arg->value, str[arg->i]);
		what_quote(str, arg);
	}
	return (arg->i);
}

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
