/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:51:52 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/06 18:13:15 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	unsetting(t_env **env, char *name)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
		{
			if (prev == NULL)
				*env = cur->next;
			else
				prev->next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	built_unset(t_env **env, char **cmd)
{
	int i = 1;

	while (cmd[i])
	{
		unsetting(env, cmd[i]);
		i++;
	}
	return (0);
}
