/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 18:32:00 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 15:40:28 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		check_special_cases(t_cmd *cmd);
int		check_for_directory(t_cmd *cmd);
void	cmd_error_and_exit_stat(t_cmd *cmd, int exit_status);
int		check_access(t_cmd *cmd);

int	check_special_cases(t_cmd *cmd)
{
	if (cmd->command[0] == '.' && !cmd->command[1])
	{
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": filename argument required", 2);
		cmd->cmd_exit = 2;
		return (1);
	}
	if (cmd->command[0] && cmd->command[1] && !cmd->command[2])
	{
		if (cmd->command[0] == '.' && cmd->command[1] == '.')
		{
			ft_putstr_fd(cmd->command, 2);
			ft_putendl_fd(": command not found", 2);
			cmd->cmd_exit = 127;
			return (1);
		}
	}
	return (0);
}

int	check_for_directory(t_cmd *cmd)
{
	struct stat	path_stat;

	if (stat(cmd->command, &path_stat) == -1)
	{
		ft_putstr_fd(cmd->command, 2);
		perror(":");
		cmd->cmd_exit = 127;
		return (-1);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": Is a directory", 2);
		cmd->cmd_exit = 126;
		return (1);
	}
	return (0);
}

void	cmd_error_and_exit_stat(t_cmd *cmd, int exit_status)
{
	ft_putstr_fd(cmd->command, 2);
	ft_putendl_fd(": Command not found", 2);
	cmd->cmd_exit = exit_status;
}

int	check_access(t_cmd *cmd)
{
	if (access(cmd->command, X_OK) == 0)
	{
		cmd->cmd_path = cmd->command;
		return (0);
	}
	if (access(cmd->command, F_OK) == 0)
	{
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": Permission denied", 2);
		cmd->cmd_exit = 126;
		return (-1);
	}
	else
		return (1);
}
