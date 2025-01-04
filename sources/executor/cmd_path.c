/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:37:17 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/03 17:01:13 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			get_cmd_path(t_shell *mini, t_cmd *cmd);
static int	process_paths(t_shell *mini, t_cmd *cmd);
static char	*get_path_from_env(t_shell *mini);
static int	no_path_var(t_cmd *cmd);
static int	check_abs_path(t_cmd *cmd);

/**
 * get_cmd_path - Determines the executable path for a command.
 *
 * @mini: Pointer to the shell structure containing environment variables.
 * @cmd: Pointer to the command structure with command details.
 *
 * Checks if the command is an absolute or relative path. If not, retrieves
 * the "PATH" environment variable, splits it into directories, and searches
 * for the command in those paths. Updates `cmd->cmd_path` with the resolved
 * path or handles errors if the command is not found or executable.
 * Returns 0 on success, 1 on failure, and sets the command's exit status.
 */
int	get_cmd_path(t_shell *mini, t_cmd *cmd)
{
	int	abs_path_status;

	if (!cmd->command || ft_strlen(cmd->command) == 0)
	{
		cmd->cmd_exit = 0;
		return (1);
	}
	abs_path_status = check_abs_path(cmd);
	if (abs_path_status == 0)
		return (0);
	if (abs_path_status != 1)
		return (cmd->cmd_exit != 0);
	return (process_paths(mini, cmd));
}

/**
 * check_abs_path - Checks if the command is an absolute or relative path.
 *
 * @cmd: Pointer to the command structure with command details.
 *
 * Determines if the command starts with '/', './', or '../' indicating
 * an absolute or relative path. Handles special cases like directories
 * and access permissions. Returns:
 *   - 0: If the command is a valid executable at the specified path.
 *   - 1: If the command is not an absolute/relative path.
 *   - -1: If the command is invalid or inaccessible.
 */
static int	check_abs_path(t_cmd *cmd)
{
	int	access_result;

	if (check_special_cases(cmd))
		return (-1);
	if (cmd->command[0] == '/' || (cmd->command[0] == '.'
			&& cmd->command[1] == '/') || (cmd->command[0] == '.'
			&& cmd->command[1] == '.' && cmd->command[2] == '/'))
	{
		if (check_for_directory(cmd) != 0)
			return (-1);
		access_result = check_access(cmd);
		if (access_result <= 0)
			return (access_result);
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": Command not found", 2);
		cmd->cmd_exit = 127;
		return (-1);
	}
	return (1);
}

/**
 * process_paths - Resolves the command's executable path
 * using the "PATH" variable.
 *
 * @mini: Pointer to the shell structure containing environment information.
 * @cmd: Pointer to the command structure containing the command to execute.
 *
 * This function retrieves the "PATH" environment variable and splits it into
 * directories. If "PATH" is missing or empty, it falls back to checking if the
 * command can be executed directly via `no_path_var`. Otherwise, it searches
 * through the directories in "PATH" for the command
 * using `search_command_in_paths`.
 * If the command is not found, it sets an error state, prints an error message,
 * and assigns a `127` exit status.
 *
 * Returns:
 * - 0 on success, if the command's executable path is resolved.
 * - 1 on failure, if the "PATH" variable is missing/empty, the command cannot
 *   be executed directly, or memory allocation fails.
 */
static int	process_paths(t_shell *mini, t_cmd *cmd)
{
	char	*paths_str;
	char	**paths;

	paths_str = get_path_from_env(mini);
	if (!paths_str || ft_strlen(paths_str) == 0)
	{
		if (no_path_var(cmd) == 0)
			return (0);
		return (1);
	}
	paths = split_paths(cmd, paths_str);
	if (!paths)
		return (1);
	cmd->cmd_path = search_command_in_paths(paths, cmd);
	if (!cmd->cmd_path)
	{
		cmd_error_and_exit_stat(cmd, 127);
		return (1);
	}
	return (0);
}

/**
 * no_path_var - Handles command execution when the "PATH" variable is absent.
 *
 * @cmd: Pointer to the command structure containing the command to execute.
 *
 * This function checks if the provided command can be executed directly
 * without relying on the "PATH" environment variable. If the command is
 * executable (using `access` with `X_OK`), it duplicates the command string
 * into `cmd_path` for execution. Otherwise, it sets an error state, prints
 * an error message, and assigns an appropriate exit status to the command.
 *
 * Returns:
 * - 0 on success, if the command is executable and `cmd_path` is set.
 * - 1 on failure, if the command cannot be executed or memory allocation fails.
 */
static int	no_path_var(t_cmd *cmd)
{
	if (access(cmd->command, X_OK) == 0)
	{
		cmd->cmd_path = ft_strdup(cmd->command);
		if (!cmd->cmd_path)
			return (1);
		return (0);
	}
	cmd_error_and_exit_stat(cmd, 127);
	return (1);
}

/**
 * get_path_from_env - Retrieves the value of the "PATH" environment variable.
 *
 * @mini: Pointer to the shell structure containing environment variables.
 *
 * Searches the environment variable list for the "PATH" variable and
 * returns its value. If not found, returns NULL.
 */
static char	*get_path_from_env(t_shell *mini)
{
	t_env	*temp;

	temp = mini->env;
	while (temp)
	{
		if (ft_strncmp(temp->name, "PATH", 4) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}
