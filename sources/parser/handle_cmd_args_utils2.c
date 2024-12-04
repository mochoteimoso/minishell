/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:18:09 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/04 16:02:46 by nzharkev         ###   ########.fr       */
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
	if ((arg->sgl == 1 && str[arg->i] == '\'') || (arg->dbl == 1 && str[arg->i] == '"'))
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

int	the_arg(t_expand *arg, int i)
{
	arg->sgl = 0;
	arg->dbl = 0;
	arg->i = i;
	arg->start = i;
	arg->value = ft_strdup("");
	if (!arg->value)
	{
		ft_putendl_fd("Malloc fail", 2);
		return (1);
	}
	return (0);
}

int	add_char(char *str, t_expand *arg)
{
	char	*temp;
	char	*temp2;

	temp2 = ft_strndup(&str[arg->i], 1);
	if (!temp2)
		return (1);
	temp = ft_strjoin(arg->value, temp2);
	free(temp2);
	if (!temp)
		return (1);
	free(arg->value);
	arg->value = temp;
	arg->i++;
	return (0);
}
