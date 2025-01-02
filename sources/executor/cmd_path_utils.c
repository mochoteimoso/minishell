/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 18:32:00 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:53:21 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		check_special_cases(t_cmd *cmd);
int		check_for_directory(t_cmd *cmd);
void	cmd_error_and_exit_stat(t_cmd *cmd, int exit_status);
int		check_access(t_cmd *cmd);

/**
 * check_special_cases - Handles special cases for commands "." and "..".
 *
 * @cmd: Pointer to the command structure containing command details.
 *
 * Checks if the command is a standalone "." or ".." and handles these cases
 * by printing appropriate error messages and setting the command's exit
 * status. Returns 1 if a special case is handled, otherwise 0.
 */
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

/**
 * check_for_directory - Checks if the given command is a directory.
 *
 * @cmd: Pointer to the command structure containing command details.
 *
 * Uses `stat` to retrieve file information. If the command is a directory,
 * prints an error message and sets the exit status to 126. If the `stat`
 * call fails, prints the error message with `perror` and sets the exit
 * status to 127. Returns:
 *   - 1: If the command is a directory.
 *   - -1: If an error occurs with `stat`.
 *   - 0: Otherwise.
 */
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

/**
 * cmd_error_and_exit_stat - Prints a command error message and sets exit status.
 *
 * @cmd: Pointer to the command structure containing command details.
 * @exit_status: Exit status to set for the command.
 *
 * Prints "command not found" for the given command and sets its exit status.
 */
void	cmd_error_and_exit_stat(t_cmd *cmd, int exit_status)
{
	ft_putstr_fd(cmd->command, 2);
	ft_putendl_fd(": Command not found", 2);
	cmd->cmd_exit = exit_status;
}

/**
 * check_access - Checks the accessibility of a command.
 *
 * @cmd: Pointer to the command structure containing command details.
 *
 * Checks if the command is executable using `access`. If the command is
 * executable, updates `cmd->cmd_path` and returns 0. If the command exists
 * but is not executable, prints a "Permission denied" error and sets the
 * exit status to 126. If the command does not exist, returns 1.
 */
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
