/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:24:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 17:04:36 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			built_env(t_shell *mini, t_cmd *cmd);
char		**env_to_array(t_env *env);
static int	finalize_env_array(t_env *temp, char **array, int i);
static char	*build_env_string(t_env *temp);
static void	free_array_on_failure(char **array, int count);

/**
 * env_to_array - Converts the environment linked list to an array of strings.
 *
 * @env: Pointer to the head of the environment linked list.
 *
 * Each environment variable is represented as a string
 * in the format `key=value`.
 * Allocates memory for the array and the strings. If an allocation fails,
 * cleans up and returns NULL.
 *
 * Returns:
 * - A pointer to the array of strings on success.
 * - NULL if memory allocation fails.
 */
char	**env_to_array(t_env *env)
{
	t_env	*temp;
	char	**array;
	int		i;
	int		len;

	temp = env;
	len = ft_lstsize((t_list *)temp);
	array = (char **)malloc(sizeof(char *) * (len + 1));
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
	if (finalize_env_array(temp, array, i))
		return (NULL);
	return (array);
}

/**
 * built_env - Prints the current environment variables to the standard output.
 *
 * @mini: Pointer to the shell structure containing environment variables.
 * @cmd: Command structure for additional arguments.
 *
 * Prints all environment variables in the format `key=value`. If extra arguments
 * are provided to the `env` command, an error message is displayed, and the
 * function exits with a status of 127.
 *
 * Returns:
 * - 0 on success.
 * - 127 if an invalid argument is passed.
 */
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

/**
 * finalize_env_array - Finalizes the construction of the environment array.
 *
 * @temp: Pointer to the last node in the environment linked list.
 * @array: Array of strings being built from the environment variables.
 * @i: Current index in the array.
 *
 * Adds the last environment variable to the array
 * and ensures it is null-terminated.
 *
 * Returns:
 * - 0 on success.
 * - 1 if memory allocation for the last variable fails.
 */
static int	finalize_env_array(t_env *temp, char **array, int i)
{
	array[i] = build_env_string(temp);
	if (!array[i])
		return (1);
	i++;
	array[i] = NULL;
	return (0);
}

/**
 * build_env_string - Constructs a `key=value` string
 * from an environment variable node.
 *
 * @temp: Pointer to the environment variable node.
 *
 * Allocates and constructs a string representing the environment variable in
 * the format `key=value`. If any allocation fails, cleans up and returns NULL.
 *
 * Returns:
 * - A newly allocated string on success.
 * - NULL if memory allocation fails.
 */
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

/**
 * free_array_on_failure - Frees partially allocated environment array.
 *
 * @array: Pointer to the array of strings.
 * @count: Number of strings successfully allocated before failure.
 *
 * Iterates through the partially allocated array, freeing each string and
 * then the array itself. Used for cleanup when an allocation error occurs.
 */
static void	free_array_on_failure(char **array, int count)
{
	while (count > 0)
	{
		free(array[count]);
		count--;
	}
	free(array);
}
