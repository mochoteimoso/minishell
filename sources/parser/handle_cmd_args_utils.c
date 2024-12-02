/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:12:21 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/02 09:17:26 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	skip_whitespace(char *str, int i)
{
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

void	what_quote(char *str, t_expand *arg)
{
	if ((arg->sgl && str[arg->i] == '\'') || (arg->dbl && str[arg->i] == '"'))
	{
		if (str[arg->i + 1] != '\0' || str[arg->i] != ' ' )
		{
			arg->sgl = 0;
			arg->dbl = 0;
			arg->i++;
			return ;
		}
		else
		{
			arg->i++;
			return ;
		}
	}
	if (str[arg->i] == '\'' && !arg->dbl)
	{
		arg->sgl = 1;
		arg->i++;
	}
	if (str[arg->i] == '"' && !arg->sgl)
	{
		arg->dbl = 1;
		arg->i++;
	}
}

int	we_have_dollar(t_shell *mini, t_expand *arg, char *str)
{
	int		s_exp;
	char	*temp;
	char	*new_res;

	s_exp = arg->i;
	temp = ft_strdup("");
	if (!temp)
		return (-1);
	arg->i = oh_its_a_dollar(mini, str, &temp, arg->i, &s_exp);
	if (arg->i == -1)
	{
		free(temp);
		return (-1);
	}
	new_res = ft_strjoin(arg->value, temp);
	if (!new_res)
	{
		free(temp);
		return (-1);
	}
	free(arg->value);
	free(temp);
	arg->value = new_res;
	return (0);
}

int	add_char(char *str, t_expand *arg)
{
	char	*temp;

	temp = ft_strjoin(arg->value, ft_strndup(&str[arg->i], 1));
	if (!temp)
		return (1);
	free(arg->value);
	arg->value = temp;
	arg->i++;
	return (0);
}

void	finalizer(t_expand *arg, char **start, int *len)
{
	*start = arg->value;
	*len = ft_strlen(arg->value);
}

int	the_arg(t_expand *arg, int i)
{
	arg->sgl = 0;
	arg->dbl = 0;
	arg->i = i;
	arg->value = ft_strdup("");
	if (!arg->value)
		return (1);
	return (0);
}

int	arg_in_quotes(t_shell *mini, char *str, int i, char **start, int *len)
{
	t_expand	arg;

	if (the_arg(&arg, i))
		return (-1);
	what_quote(str, &arg);
	while (str[arg.i])
	{
		if (str[arg.i] == ' ' && !arg.sgl && !arg.dbl)
			break ;
		if ((arg.dbl && str[arg.i] == '$') || (!arg.sgl && str[arg.i] == '$'))
		{
			if (we_have_dollar(mini, &arg, str) == -1)
				return (free(arg.value), -1);
		}
		else if (!arg.sgl && !arg.dbl && (str[arg.i] == '\'' || str[arg.i] == '"'))
			what_quote(str, &arg);
		else if ((arg.sgl && str[arg.i] == '\'') || (arg.dbl && str[arg.i] == '"'))
			what_quote(str, &arg);
		else if (add_char(str, &arg))
			return (free(arg.value), -1);
	}
	finalizer(&arg, start, len);
	return (arg.i);
}

int	arg_no_quotes(t_shell *mini, t_cmd *cmd, int i, char **start, int *len)
{
	char	*temp;
	int		s_exp;

	*start = &cmd->segment[i];
	*len = 0;
	temp = ft_strdup("");
	while (cmd->segment[i] && (!ft_isspace(cmd->segment[i]) ||
			check_quotes(cmd->segment, i)) && !is_redirection(cmd, i))
	{
		if (cmd->segment[i] == '$' || cmd->segment[i] == '~')
		{
			s_exp = i;
			i = expand_variable(mini, cmd->segment, &temp, i, &s_exp);
			*start = temp;
			*len = ft_strlen(temp);
		}
		if (cmd->segment[i] == ' ')
			return (i);
		(*len)++;
		i++;
	}
	return (i);
}

int	append_to_array(t_cmd *cmd, char *start, int len, int *index)
{
	cmd->args[*index] = ft_strndup(start, len);
	if (!cmd->args[*index])
	{
		ft_putendl_fd("Failed to allocate memory for argument", 2);
		return (-1);
	}
	(*index)++;
	return (0);
}
