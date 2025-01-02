/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 13:09:43 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			built_cd(t_shell *mini, t_cmd *cmd);
static int	to_home(char *cwd);
static int	cd_choices(t_shell *mini, t_cmd *cmd, char *cwd);
static int	old_pwd(t_shell *mini, t_cmd *cmd);
static int	to_path(t_shell *mini, char *path);

/**
 * built_cd - Handles the `cd` (change directory) command.
 *
 * @mini: The shell structure containing environment and state information.
 * @cmd: The command structure containing arguments for `cd`.
 *
 * Validates the number of arguments and determines the directory to change to.
 * Handles various `cd` cases: no arguments, `-`, or a specific path. Updates
 * the environment variables appropriately.
 *
 * Returns:
 * - 0 on success.
 * - 1 on failure.
 */
int	built_cd(t_shell *mini, t_cmd *cmd)
{
	char	*cwd;
	t_env	*pwd;

	if (ft_array_len(cmd->args) > 2)
	{
		error(mini, "too many arguments");
		return (1);
	}
	cwd = getcwd(NULL, 0);
	pwd = NULL;
	if (!cwd)
	{
		if (no_cwd(mini, pwd, &cwd))
			return (1);
	}
	if (cd_choices(mini, cmd, cwd))
	{
		free(cwd);
		return (1);
	}
	return (0);
}

/**
 * cd_choices - Determines the type of `cd` operation based on the arguments.
 *
 * @mini: The shell structure.
 * @cmd: The command structure containing arguments for `cd`.
 * @cwd: The current working directory.
 *
 * Handles different `cd` operations:
 * - No arguments: Change to the home directory.
 * - Argument `-`: Change to the old working directory (`OLDPWD`).
 * - Specific path: Change to the specified directory.
 *
 * Returns:
 * - 0 on success.
 * - 1 on failure.
 */
static int	cd_choices(t_shell *mini, t_cmd *cmd, char *cwd)
{
	if (!cmd->args[1])
	{
		if (to_home(cwd))
			return (1);
		return (0);
	}
	else if (cmd->args[1][0] == '-' && cmd->args[1][1] == '\0')
	{
		if (old_pwd(mini, cmd))
			return (0);
	}
	else
	{
		if (to_path(mini, cmd->args[1]))
			return (1);
	}
	free(cwd);
	return (0);
}

/**
 * to_home - Changes the directory to the home directory.
 *
 * @cwd: The current working directory.
 *
 * Retrieves the `HOME` environment variable and changes to that directory.
 * Prints an error if `HOME` is not set.
 *
 * Returns:
 * - 0 on success.
 * - 1 if `HOME` is not set or the change directory operation fails.
 */
static int	to_home(char *cwd)
{
	const char	*path;

	path = getenv("HOME");
	if (!path)
	{
		ft_putendl_fd("cd: no HOME", 2);
		free(cwd);
		return (1);
	}
	chdir(path);
	free(cwd);
	return (0);
}

/**
 * old_pwd - Handles the `cd -` command to change to the previous directory.
 *
 * @mini: The shell structure containing environment information.
 * @cmd: The command structure.
 *
 * Retrieves the `OLDPWD` value and changes to that directory. Updates the
 * `PWD` and `OLDPWD` environment variables.
 *
 * Returns:
 * - 0 on success.
 * - 1 on failure.
 */
static int	old_pwd(t_shell *mini, t_cmd *cmd)
{
	t_env	*env;
	char	*pwd;
	char	*oldpwd;

	if (cmd->args[1][1])
		return (1);
	env = mini->env;
	pwd = NULL;
	if (get_oldpwd(env, &pwd))
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		free(pwd);
		return (1);
	}
	chdir(pwd);
	if (handle_update_pwd(mini, pwd, oldpwd))
		return (1);
	return (0);
}

/**
 * to_path - Changes the directory to the specified path.
 *
 * @mini: The shell structure containing environment information.
 * @path: The target directory path.
 *
 * Retrieves the current directory and uses it to update the `OLDPWD` variable.
 * Attempts to change to the specified path and updates the `PWD` variable.
 *
 * Returns:
 * - 0 on success.
 * - 1 on failure.
 */
static int	to_path(t_shell *mini, char *path)
{
	char	*cwd;
	char	*oldpwd;
	t_env	*pwd;

	cwd = getcwd(NULL, 0);
	oldpwd = NULL;
	pwd = find_pwd(mini->env, "PWD");
	if (cwd)
		oldpwd = ft_strdup(cwd);
	else
	{
		if (pwd)
			oldpwd = ft_strdup(pwd->value);
		else
			oldpwd = NULL;
	}
	free(cwd);
	if (!oldpwd)
		return (ft_putendl_fd("cd: failed to get OLDPWD", 2), 1);
	if (handle_path(mini, oldpwd, path))
		return (1);
	return (0);
}
