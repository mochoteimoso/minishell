/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:37:17 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/23 16:05:42 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			get_cmd_path(t_shell *mini, t_cmd *cmd);
static char	*get_path_from_env(t_shell *mini);
static char	**split_paths(const char *paths_str);
static char	*search_command_in_paths(char **paths, t_cmd *cmd);
static int	check_abs_path(t_cmd *cmd);

// Resolves absolute path for command

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

static char	**split_paths(const char *paths_str)
{
	char	**paths;

	paths = ft_split(paths_str, ':');
	if (!paths)
		perror("Failed to split PATH");
	return (paths);
}

// Tests each possible path until finds one that works,
// returns correct absolute path

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

// Checks whether command is already an absolute path

static int	check_abs_path(t_cmd *cmd)
{
	if (check_special_cases(cmd))
		return (-1);
	if (cmd->command[0] == '/'
		|| (cmd->command[0] == '.' && cmd->command[1] == '/'))
	{
		if (check_for_directory(cmd))
			return (-1);
		if (access(cmd->command, X_OK) == 0)
		{
			cmd->cmd_path = cmd->command;
			return (0);
		}
		if (access(cmd->command, F_OK) == 0 && access(cmd->command, X_OK) != 0)
		{
			ft_putstr_fd(cmd->command, 2);
			ft_putendl_fd(": Permission denied", 2);
			cmd->cmd_exit = 126;
			return (-1);
		}
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": No such file or directory", 2);
		cmd->cmd_exit = 127;
		return (-1);
	}
	return (1);
}
