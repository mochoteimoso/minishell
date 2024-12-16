/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:12:21 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/16 15:43:00 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	skip_whitespace(char *str, int i)
{
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

int	arg_in_quotes(t_shell *mini, char *str, int i, t_expand *arg)
{
	if (the_arg(arg, i))
		return (-1);
	what_quote(str, arg);
	while (str[arg->i])
	{
		if (str[arg->i] == ' ' && !arg->sgl && !arg->dbl)
			break ;
		if (((arg->dbl && str[arg->i] == '$' && !arg->sgl) || (str[arg->i] == '$' && !arg->sgl && !arg->dbl))
			&& (str[arg->i + 1] && !ft_isspace(str[arg->i + 1]) && ((ft_isalnum(str[arg->i + 1])
			|| str[arg->i + 1] == '_' || str[arg->i + 1] == '?'))))
		{
			if (we_have_dollar(mini, arg, str) == -1)
				return (free(arg->value), -1);
		}
		else if (!arg->sgl && !arg->dbl && (str[arg->i] == '\''
			|| str[arg->i] == '"'))
			what_quote(str, arg);
		else if ((arg->sgl && str[arg->i] == '\'')
			|| (arg->dbl && str[arg->i] == '"'))
			what_quote(str, arg);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
		// printf("value: {%s}\nlen: %d\n", arg->value, (int)ft_strlen(arg->value));
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}


static int	no_expanding(t_shell *mini, char *str, t_expand *arg, int i)
{
	arg->sgl = 0;
	arg->dbl = 0;
	arg->i = i;
	arg->start = i;
	what_quote(str, arg);
	while (str[arg->i])
	{
		if ((str[arg->i] == ' ' || str[arg->i] == '\t') && !arg->sgl && !arg->dbl)
			break ;
		if ((((arg->dbl && !arg->sgl) || (!arg->dbl && !arg->sgl)) && str[arg->i] == '$' && ((str[arg->i + 1] && ft_isalnum(str[arg->i + 1])) || str[arg->i + 1] == '?')))
			i = we_have_dollar(mini, arg, str);
		else if (!arg->sgl && !arg->dbl && (str[arg->i] == '\''
			|| str[arg->i] == '"'))
			what_quote(str, arg);
		else if ((arg->sgl && str[arg->i] == '\'')
			|| (arg->dbl && str[arg->i] == '"'))
			what_quote(str, arg);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
		//printf("arg->value: {%s}\nlen: %d\n", arg->value, (int)ft_strlen(arg->value));
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

int	arg_no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg)
{
	arg->value = ft_strdup("");
	if (!arg->value)
		return (-1);
	while (cmd->segment[i] && (!ft_isspace(cmd->segment[i])
			&& !is_redirection(cmd, i)))
	{
		if (cmd->segment[i] == '$' && (cmd->segment[i + 1] == '"' || cmd->segment[i + 1] == '\''))
			i++;
		if ((cmd->segment[i] == '~' && (ft_isspace(cmd->segment[i + 1]) || cmd->segment[i + 1] == '\0')) || (cmd->segment[i] == '$'
			&& (cmd->segment[i + 1] && (ft_isalnum(cmd->segment[i + 1])
			|| cmd->segment[i + 1] == '_' || cmd->segment[i + 1] == '?'))))
		{
			arg->i = i;
			i = expand_variable(mini, cmd->segment, &arg->value, arg);
		}
		else
			i = no_expanding(mini, cmd->segment, arg, i);
		if (cmd->segment[i] == ' ')
			break ;
	}
	arg->len = ft_strlen(arg->value);
	return (i);
}

int	append_to_array(t_cmd *cmd, char *arg, int len, int *index)
{
	cmd->args[*index] = ft_strndup(arg, len);
	if (!cmd->args[*index])
	{
		ft_putendl_fd("Failed to allocate memory for argument", 2);
		return (-1);
	}
	(*index)++;
	return (0);
}
