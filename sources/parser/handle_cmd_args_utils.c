/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:12:21 by henbuska          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/12/04 14:46:44 by henbuska         ###   ########.fr       */
=======
/*   Updated: 2024/12/02 16:49:01 by nzharkev         ###   ########.fr       */
>>>>>>> main
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
<<<<<<< HEAD
		if (((arg->dbl && str[arg->i] == '$') || (!arg->sgl && str[arg->i] == '$'))
			&& (str[arg->i + 1] && ((ft_isalnum(str[arg->i + 1]) || str[arg->i + 1] == '_' || str[arg->i + 1] == '?'))))
=======
		if ((arg->dbl && str[arg->i] == '$') || (!arg->sgl && str[arg->i] == '$'))
>>>>>>> main
		{
			if (we_have_dollar(mini, arg, str) == -1)
				return (free(arg->value), -1);
		}
		else if (!arg->sgl && !arg->dbl && (str[arg->i] == '\'' || str[arg->i] == '"'))
			what_quote(str, arg);
		else if ((arg->sgl && str[arg->i] == '\'') || (arg->dbl && str[arg->i] == '"'))
			what_quote(str, arg);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

static int	no_expanding(t_cmd *cmd, t_expand *arg, int i)
{
	char	*temp;
	char	*temp2;

<<<<<<< HEAD
	if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
		return(i);
=======
>>>>>>> main
	temp2 = ft_strndup(&cmd->segment[i], 1);
	if (!temp2)
		return (-1);
	temp = ft_strjoin(arg->value, temp2);
	free(temp2);
	if (!temp)
		return (-1);
	free(arg->value);
	arg->value = temp;
	return (i);
}

int	arg_no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg)
{
	arg->value = ft_strdup("");
	if (!arg->value)
		return (-1);
	while (cmd->segment[i] && (!ft_isspace(cmd->segment[i])
<<<<<<< HEAD
			&& !is_redirection(cmd, i)))
	{
		if ((cmd->segment[i] == '$' || cmd->segment[i] == '~')
			&& (cmd->segment[i + 1] && ((ft_isalnum(cmd->segment[i + 1]) || cmd->segment[i + 1] == '_' || cmd->segment[i + 1] == '?'))))
=======
			|| check_quotes(cmd->segment, i)) && !is_redirection(cmd, i))
	{
		if (cmd->segment[i] == '$' || cmd->segment[i] == '~')
>>>>>>> main
		{
			arg->i = i;
			i = expand_variable(mini, cmd->segment, &arg->value, arg);
		}
		else
		{
			i = no_expanding(cmd, arg, i);
			i++;
		}
		if (cmd->segment[i] == ' ')
			break ;
	}
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
