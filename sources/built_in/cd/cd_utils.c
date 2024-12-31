/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:40:06 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 17:07:14 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	get_oldpwd(t_env *env, char **pwd);
int	update_env_value(t_env *env, char *new_value);
int	update_pwd(t_env *env, char *wd, char **oldpwd);
int	handle_update_pwd(t_shell *mini, char *pwd, char *oldpwd);

/**
 * get_oldpwd - Retrieves the value of the `OLDPWD` environment variable.
 *
 * @env: Pointer to the environment variable linked list.
 * @pwd: Pointer to a string where the value of `OLDPWD` will be stored.
 *
 * Searches the environment variable list for `OLDPWD`
 * and prints its value to the standard output.
 * Allocates memory to store the value in `pwd`.
 *
 * Returns:
 * - 0 on success (with `pwd` updated).
 * - 1 on failure (if memory allocation fails or `OLDPWD` is not found).
 */
int	get_oldpwd(t_env *env, char **pwd)
{
	while (env)
	{
		if (ft_strcmp(env->name, "OLDPWD") == 0)
		{
			printf("%s\n", env->value);
			*pwd = ft_strdup(env->value);
			if (!*pwd)
				return (1);
		}
		env = env->next;
	}
	return (0);
}

/**
 * update_env_value - Updates the value of a given environment variable.
 *
 * @env: Pointer to the environment variable node.
 * @new_value: The new value to set for the environment variable.
 *
 * Frees the existing value and replaces it with the new value.
 *
 * Returns:
 * - 0 on success.
 * - 1 on failure (if memory allocation for the new value fails).
 */
int	update_env_value(t_env *env, char *new_value)
{
	char	*temp;

	temp = ft_strdup(new_value);
	if (!temp)
		return (1);
	free(env->value);
	env->value = temp;
	return (0);
}

/**
 * update_pwd - Updates the `PWD` and `OLDPWD` environment variables.
 *
 * @env: Pointer to the environment variable linked list.
 * @wd: The new working directory to set for `PWD`.
 * @oldpwd: Pointer to the previous working directory value,
 * 			used to update `OLDPWD`.
 *
 * Iterates through the environment list to find `PWD`
 * and `OLDPWD` and updates their values
 * with the provided arguments. Stops early if both are updated.
 *
 * Returns:
 * - 0 on success.
 * - 1 on failure (if memory allocation for the updated values fails).
 */
int	update_pwd(t_env *env, char *wd, char **oldpwd)
{
	int	flg;

	flg = 0;
	while (env)
	{
		if (ft_strcmp(env->name, "OLDPWD") == 0
			&& update_env_value(env, *oldpwd))
			return (1);
		if (ft_strcmp(env->name, "PWD") == 0)
		{
			if (update_env_value(env, wd))
				return (1);
			flg += 1;
		}
		if (flg == 2)
			break ;
		env = env->next;
	}
	return (0);
}

/**
 * handle_update_pwd - Handles updating `PWD`
 * and `OLDPWD` in the shell's environment.
 *
 * @mini: The shell structure containing environment information.
 * @pwd: The new working directory value for `PWD`.
 * @oldpwd: The previous working directory value for `OLDPWD`.
 *
 * Calls `update_pwd` to update `PWD` and `OLDPWD` in the shell's environment.
 * Frees allocated resources and handles error reporting if `OLDPWD` is not set.
 *
 * Returns:
 * - 0 on success.
 * - 1 on failure (if updates or memory allocations fail).
 */
int	handle_update_pwd(t_shell *mini, char *pwd, char *oldpwd)
{
	if (update_pwd(mini->env, pwd, &oldpwd))
	{
		free(pwd);
		free(oldpwd);
		return (1);
	}
	if (!oldpwd)
		error(mini, "No OLDPWD set");
	free(oldpwd);
	free(pwd);
	return (0);
}
