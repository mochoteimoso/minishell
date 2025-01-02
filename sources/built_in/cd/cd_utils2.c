/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:14:49 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:43:10 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	no_cwd(t_shell *mini, t_env *pwd, char **cwd);
int	handle_path(t_shell *mini, char *oldpwd, char *path);

/**
 * no_cwd - Fallback mechanism when `getcwd` fails
 * 			to retrieve the current working directory.
 *
 * @mini: The shell structure containing environment information.
 * @pwd: Pointer to the `PWD` environment variable.
 * @cwd: Pointer to the string where the current working directory is stored.
 *
 * Attempts to use the value of the `PWD` environment variable as the current
 * directory. Prints an error if both `getcwd` and `PWD` are unavailable or if
 * memory allocation fails.
 *
 * Returns:
 * - 0 on success (with `cwd` updated).
 * - 1 on failure (with an error message printed).
 */

int	no_cwd(t_shell *mini, t_env *pwd, char **cwd)
{
	pwd = find_pwd(mini->env, "PWD");
	if (pwd && pwd->value)
	{
		*cwd = ft_strdup(pwd->value);
		if (!(*cwd))
		{
			error(mini, "cd: malloc failed");
			return (1);
		}
	}
	else
	{
		error(mini, "getcwd: cannot access parent directories:\
		No such file or directory");
		return (1);
	}
	return (0);
}

/**
 * handle_path - Changes the directory to a specified path
 * 				and updates environment variables.
 *
 * @mini: The shell structure containing environment information.
 * @oldpwd: The previous working directory, used to update `OLDPWD`.
 * @path: The target directory path.
 *
 * Attempts to change to the specified directory. If successful, retrieves the
 * new current working directory and updates the `PWD` and `OLDPWD` environment
 * variables. Handles cases where `getcwd` fails by using the `path` argument as
 * a fallback.
 *
 * Returns:
 * - 0 on success.
 * - 1 on failure (with `oldpwd` and other resources freed as needed).
 */

int	handle_path(t_shell *mini, char *oldpwd, char *path)
{
	char	*newcwd;

	if (chdir(path) == -1)
	{
		ft_putendl_fd("cd: no such file or directory", 2);
		free(oldpwd);
		return (1);
	}
	newcwd = getcwd(NULL, 0);
	if (!newcwd)
		newcwd = ft_strdup(path);
	if (update_pwd(mini->env, newcwd, &oldpwd))
	{
		free(newcwd);
		free(oldpwd);
		return (1);
	}
	free(newcwd);
	free(oldpwd);
	return (0);
}
