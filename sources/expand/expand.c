/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:34:39 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/11 16:10:38 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	expand_variable(t_shell *mini, char *str, int *i)
{
	char *name;
	int j;
	j = 0;
	while (!is_space(str))
	{
		name[j] = str[i];
		i++;
		j++;
	}
	while (mini->env->name != name)
		env = env->next;
	mini->cmds->args[i] = mini->env->value;
}
if (str[i] == '$' && !is_in_quotes(str[i])
{
	i++
	expand_variable(cmd, str);
}
