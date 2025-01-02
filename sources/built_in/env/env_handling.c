/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:14:43 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:52:16 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_env	*list_env(char **envp);
char	**copy_env(char **envp);
void	to_alphabetical(char **array);

/**
 * list_env - Creates a linked list of environment variables.
 *
 * @envp: Array of strings representing
 * the environment variables in "NAME=VALUE" format.
 *
 * Iterates over the environment variable array and creates a linked list where
 * each node represents a single environment variable. Each node is created using
 * the `add_node` function and appended to the list using `ft_env_lstadd_back`.
 *
 * Returns:
 * - A pointer to the head of the linked list.
 * - NULL if any node creation fails.
 */
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
		if (!new_node)
			return (NULL);
		ft_env_lstadd_back(&ll, new_node);
		i++;
	}
	return (ll);
}

/**
 * copy_env - Creates a copy of the environment variable array.
 *
 * @envp: Array of strings representing the environment variables.
 *
 * Allocates a new array of strings and copies each environment variable from
 * `envp` into it using `ft_strdup`. Handles memory allocation errors by freeing
 * already allocated memory.
 *
 * Returns:
 * - A pointer to the newly created array of strings.
 * - NULL if any memory allocation fails.
 */
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
		if (!copy[i])
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

/**
 * to_alphabetical - Sorts an array of strings in alphabetical order.
 *
 * @array: Array of strings to sort.
 *
 * Implements a simple bubble sort algorithm to arrange the strings in ascending
 * alphabetical order. Swaps adjacent elements until the array is fully sorted.
 */
void	to_alphabetical(char **array)
{
	char	*temp;
	int		i;

	i = 1;
	while (array[i])
	{
		if (ft_strcmp(array[i], array[i -1]) < 0)
		{
			temp = array[i];
			array[i] = array[i - 1];
			array[i - 1] = temp;
			i = 0;
		}
		i++;
	}
}
