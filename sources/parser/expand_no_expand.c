/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_no_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:29:31 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/30 19:21:58 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			no_expanding(t_shell *mini, char *str, t_expand *arg);
static int	this_breaks(t_expand *arg, char *str);
static int	no_expand_dollar(t_expand *arg, char *str);

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

static int	this_breaks(t_expand *arg, char *str)
{
	if (ft_isspace(str[arg->i]) || str[arg->i] == '\'' || str[arg->i] == '"')
		return (1);
	else
		return (0);
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
