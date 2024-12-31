/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:18:09 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 11:16:26 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_question(t_shell *mini, char *str, char **expanded, t_expand *arg)
{
	int	cont;
	arg->start = arg->i;
	cont = arg->start;
	if (str[arg->i] == '$')
		arg->i = oh_its_a_dollar(mini, str, expanded, arg);
	arg->i = cont + 2;
	return (arg->i);
}

int	new_result(t_expand *arg, char *temp)
{
	char	*new_res;

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

int	we_have_dollar(t_shell *mini, t_expand *arg, char *str)
{
	int		s_exp;
	char	*temp;
	char	*new_res;

	s_exp = arg->i;
	temp = ft_strdup("");
	if (!temp)
		return (-1);
	arg->start = arg->i;
	arg->i = oh_its_a_dollar(mini, str, &temp, arg);
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

void	what_quote(char *str, t_expand *arg)
{
	if ((arg->sgl == 1 && str[arg->i] == '\'') || (arg->dbl == 1
		&& str[arg->i] == '"'))
	{
		if (is_redirection(cmd, i))
			i = count_if_redirection(cmd, ++i);
		else
		{
			arg->i++;
			return ;
		}
	}
	if (str[arg->i] == '\'' && arg->sgl == 0)
	{
		arg->sgl = 1;
		arg->i++;
		return ;
	}
	if (str[arg->i] == '"' && arg->dbl == 0)
	{
		arg->dbl = 1;
		arg->i++;
		return ;
	}
}

int	count_if_redirection(t_cmd *cmd, int i)
{
	while (cmd->seg[i] && ft_isspace(cmd->seg[i]))
		i++;
	while (cmd->seg[i] && !ft_isspace(cmd->seg[i])
		&& is_redirection(cmd, i))
		i++;
	return (i);
}

int	only_redirect(char *str, int i)
{
	int	n;

	n = i;
	while (str[n] && str[n] != '=')
		n++;
	if (!str[n])
		return (0);
	n++;
	if (str[n])
		n++;
	if ((str[n] == '>' || str[n] == '<') || str[n] == '|')
		return (1);
	return (0);
}

int	init_args_array(t_cmd *cmd, int i)
{
	cmd->a_num = count_args(cmd, i) + 1;
	cmd->args = ft_calloc(cmd->a_num + 1, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[0] = ft_strdup(cmd->command);
	if (!cmd->args[0])
	{
		ft_free_array(cmd->args);
		return (-1);
	}
	return (0);
}


