/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:51:52 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/26 15:08:22 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			built_unset(t_shell *mini, t_cmd *cmd);
static int	unset_env(t_shell *mini, char *str);
static int	unset_pending(t_shell *mini, char *str);

int	built_unset(t_shell *mini, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (cmd->a_num >= 2)
	{
		while (i < cmd->a_num)
		{
			if (cmd->args[i][0] == '-')
			{
				ft_putendl_fd("Invalid option", 2);
				return (2);
			}
			if (unset_env(mini, cmd->args[i]))
			{
				error(mini, "No such variable");
				return (1);
			}
			if (unset_pending(mini, cmd->args[i]))
			{
				error(mini, "No such variable");
				return (1);
			}
			i++;
		}
	}
	return (0);
}

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
				mini->env = cur->next;
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

