/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:13:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/12 18:01:12 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	clean_env(t_env *ll, char **array)
{
	t_env *temp;

	if (array)
		ft_free_array(array);
	while (ll != NULL)
	{
		temp = ll;
		free(ll->name);
		free(ll->value);
		ll = ll->next;
		free(temp);
	}
}

static void	clean_redir(t_redir *head, t_redir *tail)
{
	t_redir *temp;

	if (head)
	{
		while (head != NULL)
		{
			temp = head;
			free(head->file);
			free(head->delimiter);
			head = head->next;
			free(temp);
		}
	}
	if (tail)
	{
		while (tail != NULL)
		{
			temp = tail;
			free(tail->file);
			free(tail->delimiter);
			head = tail->next;
			free(temp);
		}
	}
}

static void	clean_cmds(t_cmd **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		free(cmds[i]->segment);
		free(cmds[i]->command);
		free(cmds[i]->cmd_path);
		ft_free_array(cmds[i]->args);
		clean_redir(cmds[i]->redir_head,  cmds[i]->redir_tail);
		i++;
	}
	free(cmds);
}

void	cleaner(t_shell *mini)
{
	clean_env(mini->env, mini->pending);
	clean_cmds(mini->cmds);
}

void	error(char *str)
{
	ft_putstr_fd(str, 2);
}
