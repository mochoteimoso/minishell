/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:24:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/23 18:26:25 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	built_env(t_shell *mini, t_cmd *cmd)
{
	t_env	*temp;

	temp = mini->env;
	if (cmd->args[1])
	{
		ft_putendl_fd("No such file or directory", 2);
		mini->exit_stat = 127;
		return (127);
	}
	while (temp != NULL)
	{
		printf("%s=%s\n", temp->name, temp->value);
		temp = temp->next;
	}
	return (0);
}

static char	*build_env_string(t_env *temp)
{
	char	*result;
	char	*inter;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	inter = ft_strjoin(result, temp->name);
	free(result);
	if (!inter)
		return (NULL);
	result = inter;
	inter = ft_strjoin(result, "=");
	free(result);
	if (!inter)
		return (NULL);
	result = inter;
	inter = ft_strjoin(result, temp->value);
	free(result);
	if (!inter)
		return (NULL);
	result = inter;
	return (result);
}

static void	free_array_on_failure(char **array, int count)
{
	while (count > 0)
	{
		free(array[count]);
		count--;
	}
	free(array);
}

char	**env_to_array(t_env *env)
{
	t_env	*temp;
	char	**array;
	int		i;
	int		len;

	temp = env;
	len = ft_lstsize((t_list *)temp);
	array = (char **)malloc(sizeof(char *) * len + 2);
	if (!array)
		return (NULL);
	i = 0;
	while (temp->next)
	{
		array[i] = build_env_string(temp);
		if (!array[i])
		{
			free_array_on_failure(array, i);
			return (NULL);
		}
		temp = temp->next;
		i++;
	}
	array[i] = build_env_string(temp);
	i++;
	array[i] = NULL;
	return (array);
}
