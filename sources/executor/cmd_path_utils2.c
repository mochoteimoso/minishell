/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:21:48 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/03 16:36:47 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*search_command_in_paths(char **paths, t_cmd *cmd);
char	**split_paths(t_cmd *cmd, const char *paths_str);

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
char	*search_command_in_paths(char **paths, t_cmd *cmd)
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
 * split_paths - Splits the "PATH" environment variable into directories.
 *
 * @paths_str: The string containing the colon-separated paths.
 *
 * Uses `ft_split` to separate the paths by ':' and returns an array of
 * directory strings. If splitting fails, prints an error message and
 * returns NULL.
 */
char	**split_paths(t_cmd *cmd, const char *paths_str)
{
	char	**paths;

	if (!paths_str)
	{
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": No such file or directory", 2);
		cmd->cmd_exit = 127;
		return (NULL);
	}
	paths = ft_split(paths_str, ':');
	if (!paths || !paths_str)
		perror("Failed to split PATH");
	return (paths);
}
