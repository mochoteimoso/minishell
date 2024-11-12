/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:37:17 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/12 18:32:39 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			get_cmd_path(t_shell *mini, t_cmd *cmd);
static char	*search_command_in_paths(char **paths, t_cmd *cmd);
static int	check_abs_path(t_cmd *cmd);

/*void	execute_command(char *cmd, char **envp)
{
	char	**cmd_array;
	char	*cmd_path;

	cmd_array = NULL;
	cmd_path = NULL;
	parse_and_validate_cmd(cmd, &cmd_array);
	resolve_cmd_path(cmd_array, &cmd_path, envp);
	if (execve(cmd_path, cmd_array, envp) == -1)
	{
		perror(cmd);
		free_array(cmd_array);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
} */

int	get_cmd_path(t_shell *mini, t_cmd *cmd)
{
	char	*paths_str;
	char	**paths;
	t_env	*temp;

	if (check_abs_path(cmd) == 0)
		cmd->cmd_path = cmd->command;
	else if (check_abs_path(cmd) != 1)
		return (check_abs_path(cmd));
	else
	{
		temp = mini->env;
		while (temp)
		{
			if (ft_strncmp(temp->name, "PATH", 4) == 0)
				paths_str = temp->value;
			temp = temp->next;
		}
		paths = ft_split(paths_str, ':');
		if (!paths)
		{
			perror("Failed to split PATH");
			return (1);
		}
		cmd->cmd_path = search_command_in_paths(paths, cmd);
		if (!cmd->cmd_path)
			return (1);
	}
	return (0);
}

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

static int	check_abs_path(t_cmd *cmd)
{
	if (cmd->command[0] == '/' || cmd->command[0] == '.')
	{
		if (access(cmd->command, X_OK) == 0)
			return (0);
		if (access(cmd->command, F_OK) == 0 && access(cmd->command, X_OK) != 0)
		{
			//error_no_permission(cmd->command);
			return (126);
		}
		else
		{
			perror(cmd->command);
			return (127);
		}
	}
	return (1);
}
