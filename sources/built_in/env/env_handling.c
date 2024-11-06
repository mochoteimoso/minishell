/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:14:43 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/05 17:42:27 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_env	*list_env(char **envp)
{
	t_env	*ll;
	t_env	*new_node;
	int		i;

	i = 0;
	ll = NULL;
	while (envp[i])
	{
		new_node = add_node(envp[i]);
		ft_env_lstadd_back(&ll, new_node);
		i++;
	}
	return (ll);
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
