/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:10:30 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:53:04 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		built_pwd(t_shell *mini);
t_env	*find_pwd(t_env *env, char *name);

/**
 * built_pwd - Prints the current working directory or fallback PWD value.
 *
 * @mini: Pointer to the shell structure containing environment variables.
 *
 * Attempts to retrieve and print the current working directory using `getcwd`.
 * If `getcwd` fails, it falls back to the `PWD` variable in the environment,
 * if it exists and has a value. If neither method works,
 * an error message is displayed.
 *
 * Returns:
 * - 0 on success (whether using `getcwd` or the `PWD` environment variable).
 * - 1 if neither the current working directory
 * 	nor the `PWD` variable can be accessed.
 */
int	built_pwd(t_shell *mini)
{
	char	*cwd;
	t_env	*pwd;

	cwd = getcwd(NULL, 0);
	pwd = find_pwd(mini->env, "PWD");
	if (!cwd)
	{
		if (mini->env)
		{
			if (pwd && pwd->value)
			{
				printf("%s\n", pwd->value);
				return (0);
			}
		}
		ft_putendl_fd("pwd: cannot access current directory", 2);
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

/**
 * find_pwd - Searches for the `PWD` environment variable in a linked list.
 *
 * @env: Pointer to the head of the environment variables linked list.
 * @name: Name of the variable to find (e.g., "PWD").
 *
 * Traverses the linked list of environment variables and returns the node
 * containing the specified variable name.
 *
 * Returns:
 * - Pointer to the `t_env` node if the variable is found.
 * - `NULL` if the variable is not found in the list.
 */
t_env	*find_pwd(t_env *env, char *name)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}
