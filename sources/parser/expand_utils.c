/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:12:11 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/21 20:20:44 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			in_quotes(t_shell *mini, char *str, int i, t_expand *arg);
static int	handle_dollar(t_shell *mini, t_expand *arg, char *str);
static int	handle_quotes(t_expand *arg, char *str);
int			we_have_heredoc(t_expand *arg, char *str);

int	in_quotes(t_shell *mini, char *str, int i, t_expand *arg)
{
	char	*temp;

	if (the_arg(arg, i))
		return (-1);
	temp = arg->value;
	arg->value = ft_strjoin_char(temp, str[arg->i]);
	free(temp);
	what_quote(str, arg);
	while (str[arg->i])
	{
		if (!arg->sgl && !arg->dbl)
			break ;
		if (str[arg->i] == '$')
			arg->i = handle_dollar(mini, arg, str);
		else if (str[arg->i] == '\'' || str[arg->i] == '"')
			arg->i = handle_quotes(arg, str);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

int	we_have_heredoc(t_expand *arg, char *str)
{
	arg->value = ft_strdup("");
	while (str[arg->i] == '<' || str[arg->i] == ' ')
		add_char(str, arg);
	while (str[arg->i] && (!ft_isspace(str[arg->i])
			|| !(str[arg->i] == '<') || !(str[arg->i] == '>')))
		add_char(str, arg);
	return (arg->i);
}

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
			return (free(arg->value), -1);
	}
	return (arg->i);
}

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
