/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:51:52 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/12 17:03:51 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	unset_env(t_shell *mini, char *str)
{
	t_env	*cur;
	t_env	*prev;

	cur = mini->env;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->name, str) == 0)
		{
			if (!prev)
				prev = cur->next;
			else
				prev->next = cur->next;
		free(cur->name);
		free(cur->value);
		free(cur);
		return (0);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

static int	unset_pending(t_shell *mini, char *str)
{
	int	i;
	int	j;
	int	len;

	len = ft_strlen(str);
	i = 0;
	while (mini->pending && mini->pending[i])
	{
		if (ft_strncmp(mini->pending[i], str, len) == 0)
		{
			free(mini->pending[i]);
			break ;
		}
		i++;
	}
	j = i;
	while (mini->pending[j] && mini->pending[j + 1])
	{
		mini->pending[j] = mini->pending[j + 1];
		j++;
	}
	mini->pending[j] = NULL;
	return (0);
}

int	built_unset(t_shell *mini, t_cmd *cmd)
{
	int	sum;
	int	i;

	sum = 0;
	i = 1;
	while (cmd->args[sum])
		sum++;
	if (sum >= 2)
	{
		while (i < sum)
		{
			if (unset_env(mini, cmd->args[i]))
			{
				error("No such variable\n");
				return (1);
			}
			if (unset_pending(mini, cmd->args[i]))
			{
				error("No such variable\n");
				return (1);
			}
			i++;
		}
	}
	return (0);
}
