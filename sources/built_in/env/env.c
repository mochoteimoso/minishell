/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:24:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/14 14:19:58 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	built_env(t_shell *mini)
{
	t_env *temp = mini->env;

	while (temp != NULL)
	{
		printf("%s=%s\n", temp->name, temp->value);
		temp = temp->next;
	}
	return (0);
}

char	**env_to_array(t_env *env)
{
	t_env	*temp;
	char	**array;
	int		i;
	int		len;

	temp = env;
	len = ft_lstsize((t_list *)temp);
	temp = env;
	i = 0;
	array = (char **)malloc(sizeof(char *) * len + 1);
	while (temp->next)
	{
		array[i] = ft_strdup("");
		array[i] = ft_strjoin(array[i], temp->name);
		array[i] = ft_strjoin(array[i], "=");
		array[i] = ft_strjoin(array[i], temp->value);
		temp = temp->next;
		printf("%s\n", array[i]);
		i++;
	}
	// add error handling
	array[i] = NULL;
	return (array);
}
