/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:37:17 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 16:36:53 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			get_cmd_path(t_shell *mini, t_cmd *cmd);
static char	*get_path_from_env(t_shell *mini);
static char	**split_paths(const char *paths_str);
static char	*search_command_in_paths(char **paths, t_cmd *cmd);
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
	char	*paths_str;
	char	**paths;
	int		abs_path_status;

	if (cmd->command == NULL || ft_strlen(cmd->command) == 0)
	{
		cmd->cmd_exit = 0;
		return (1);
	}
	abs_path_status = check_abs_path(cmd);
	if (abs_path_status == 0)
		return (0);
	if (abs_path_status != 1)
		return (cmd->cmd_exit != 0);
	paths_str = get_path_from_env(mini);
	paths = split_paths(paths_str);
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

/**
 * split_paths - Splits the "PATH" environment variable into directories.
 *
 * @paths_str: The string containing the colon-separated paths.
 *
 * Uses `ft_split` to separate the paths by ':' and returns an array of
 * directory strings. If splitting fails, prints an error message and
 * returns NULL.
 */
static char	**split_paths(const char *paths_str)
{
	char	**paths;

	paths = ft_split(paths_str, ':');
	if (!paths)
		perror("Failed to split PATH");
	return (paths);
}

/**
 * search_command_in_paths - Searches for a command in the given paths.
 *
 * @paths: Array of directory paths to search.
 * @cmd: Pointer to the command structure with command details.
 *
 * Constructs potential paths for the command by appending the command
 * name to each directory in `paths`. Checks for executable access to each
 * constructed path. Frees resources and returns the valid path if found,
 * or NULL if not found.
 */
static char	*search_command_in_paths(char **paths, t_cmd *cmd)
{
	char	*cmd_path;
	char	*temp_cmd;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp_cmd = ft_strjoin(paths[i], "/");
		if (!temp_cmd)
			break ;
		cmd_path = ft_strjoin(temp_cmd, cmd->command);
		free(temp_cmd);
		if (!cmd_path)
			break ;
		if (access(cmd_path, X_OK) == 0)
		{
			ft_free_array(paths);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
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
