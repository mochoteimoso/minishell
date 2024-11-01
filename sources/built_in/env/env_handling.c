/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:14:43 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/01 14:37:43 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	*list_env(t_env *ll, char **envp)
{
	t_env	*temp;
	int		i;

	i = 0;
	if (ll == NULL)
		ll = add_node(envp[i++]);
	temp = ll;
	while (envp[i])
		ft_env_lstadd_back(ll, add_node(envp[i++]));
	ll = temp;
	return (0);
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
