/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 18:32:00 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/27 19:06:26 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_special_cases(t_cmd *cmd)
{
	if (cmd->command[0] == '~' && !cmd->command[1])
	{
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": Is a directory", 2);
		cmd->cmd_exit = 126;
		return (1);
	}
	if (cmd->command[0] == '.' && !cmd->command[1])
	{
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": filename argument required", 2);
		cmd->cmd_exit = 127;
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
	int	fd;

	fd = open(cmd->command, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
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
