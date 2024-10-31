/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:14:43 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/31 17:26:58 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_env	*create_node(void)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	return (node);
}

static t_env	*add_node(void *stuff)
{
	t_env	*node;
	char	**temp;

	node = create_node();
	if (set_name(node, stuff) == 1)
	{
		free(node);
		return (NULL);
	}
	temp = ft_split(stuff, '=');
	if (!temp)
	{
		cleaner(node, NULL);
		return (NULL);
	}
	if (set_value(node, temp) == 1)
	{
		cleaner(node, temp);
		return (NULL);
	}
	node->next = NULL;
	ft_free_array(temp);
	return (node);
}

void	*list_env(t_env *ll, char **envp)
{
	t_env	*temp;
	int		i;

	i = 0;
	if (ll == NULL)
		ll = add_node(envp[i++]);
	temp = ll;
	while (envp[i])
		ft_lstadd_back(ll, add_node(envp[i++]));
	ll = temp;
}

char	**copy_env(char **envp)
{
	int		i;
	char	**copy;

	copy = (char **)malloc(sizeof(char *) * (ft_array_len(envp) + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy)
		{
			while (i > 0)
				free(copy[--i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
