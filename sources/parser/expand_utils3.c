/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:55:47 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/27 15:13:06 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		init_expansion(t_expand *arg, char **expan);
int		the_arg(t_expand *arg, int i);
void	what_quote(char *str, t_expand *arg);
int		handle_question(t_shell *mini, char *str, char **expan, t_expand *arg);
int		new_result(t_expand *arg, char *temp);

int	init_expansion(t_expand *arg, char **expan)
{
	*expan = ft_strdup("");
	if (!(*expan))
		return (1);
	the_arg(arg, 0);
	if (!arg->value)
		return (1);
	free(arg->value);
	return (0);
}

int	the_arg(t_expand *arg, int i)
{
	arg->sgl = 0;
	arg->dbl = 0;
	arg->i = i;
	arg->start = i;
	arg->name = NULL;
	arg->value = ft_strdup("");
	if (!arg->value)
	{
		ft_putendl_fd("Malloc fail", 2);
		return (1);
	}
	return (0);
}

void	what_quote(char *str, t_expand *arg)
{
	if ((arg->sgl == 1 && str[arg->i] == '\'') || (arg->dbl == 1
			&& str[arg->i] == '"'))
	{
		if (str[arg->i] == '\'')
			arg->sgl = !arg->sgl;
		else if (str[arg->i] == '\"')
			arg->dbl = !arg->dbl;
		arg->i++;
		return ;
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

int	handle_question(t_shell *mini, char *str, char **expan, t_expand *arg)
{
	int	cont;

	arg->start = arg->i;
	cont = arg->start;
	if (str[arg->i] == '$')
		arg->i = oh_a_dollar(mini, str, expan, arg);
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
