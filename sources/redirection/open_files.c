/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:18:57 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:55:56 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		open_input_file(t_cmd *cmd, char *input_file);
int		open_output_file(t_cmd *cmd, char *output_file);
int		open_append_file(t_cmd *cmd, char *output_file);
int		open_heredoc(t_cmd *cmd, char *delimiter);
void	error_and_update_ex(t_cmd *cmd, char *file, char *error, int ex);

/**
 * open_input_file - Opens an input file for reading.
 *
 * @cmd: Pointer to the command structure to update in case of an error.
 * @file: Path to the input file to open.
 *
 * Opens the specified file with `O_RDONLY` mode. If the file does not exist,
 * or if the user lacks read permissions, prints an error message
 * and updates the command's exit status.
 * Returns the file descriptor on success, or -1 on failure.
 */
int	open_input_file(t_cmd *cmd, char *file)
{
	int	fd_in;

	fd_in = open(file, O_RDONLY);
	if (fd_in == -1)
	{
		if (access(file, F_OK) != 0)
		{
			error_and_update_ex(cmd, file, "No such file or directory", 1);
			return (-1);
		}
		if (access(file, R_OK) != 0)
		{
			ft_putstr_fd(file, 2);
			error_and_update_ex(cmd, file, "Permission denied", 126);
			return (-1);
		}
	}
	return (fd_in);
}

/**
 * open_output_file - Opens an output file for writing.
 *
 * @cmd: Pointer to the command structure to update in case of an error.
 * @output_file: Path to the output file to open.
 *
 * Opens the specified file for writing,
 * truncating its contents if it already exists.
 * If the file is a directory or lacks write permissions,
 * prints an error message and updates the command's exit status.
 * Creates the file with permissions `0644`
 * if it doesn't exist. Returns the file descriptor on success, or -1 on failure.
 */
int	open_output_file(t_cmd *cmd, char *output_file)
{
	int	fd_out;

	fd_out = open(output_file, O_DIRECTORY);
	if (fd_out != -1)
	{
		close(fd_out);
		error_and_update_ex(cmd, output_file, "Is a directory", 1);
		return (-1);
	}
	if (access(output_file, F_OK) == 0 && access(output_file, W_OK) == -1)
	{
		error_and_update_ex(cmd, output_file, "Permission denied", 1);
		return (-1);
	}
	fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		error_and_update_ex(cmd, output_file, "No such file or directory", 1);
		return (-1);
	}
	return (fd_out);
}

/**
 * open_append_file - Opens an output file for appending.
 *
 * @cmd: Pointer to the command structure to update in case of an error.
 * @file: Path to the file to open for appending.
 *
 * Opens the specified file for appending data.
 * If the file is a directory or lacks write permissions,
 * prints an error message and updates the command's exit status.
 * Creates the file with permissions `0644` if it doesn't exist.
 * Returns the file descriptor on success, or -1 on failure.
 */
int	open_append_file(t_cmd *cmd, char *file)
{
	int	fd_out;

	fd_out = open(file, O_DIRECTORY);
	if (fd_out != -1)
	{
		close(fd_out);
		error_and_update_ex(cmd, file, "Is a directory", 1);
		return (-1);
	}
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
	{
		error_and_update_ex(cmd, file, "Permission denied", 1);
		return (-1);
	}
	fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		error_and_update_ex(cmd, file, "No such file or directory", 1);
		return (-1);
	}
	return (fd_out);
}

/**
 * open_heredoc - Opens a heredoc file for reading.
 *
 * @cmd: Pointer to the command structure to update in case of an error.
 * @file: Path to the heredoc file to open.
 *
 * Opens the specified heredoc file in `O_RDONLY` mode.
 * If the file does not exist or if the user lacks read permissions,
 * prints an error message and updates the command's exit status.
 * Returns the file descriptor on success, or -1 on failure.
 */
int	open_heredoc(t_cmd *cmd, char *file)
{
	int	fd_in;

	fd_in = open(file, O_RDONLY);
	if (fd_in == -1)
	{
		if (access(file, F_OK) != 0)
		{
			error_and_update_ex(cmd, file, "No such file or directory", 1);
			return (-1);
		}
		if (access(file, R_OK) != 0)
		{
			error_and_update_ex(cmd, file, "Permission denied", 1);
			return (-1);
		}
	}
	return (fd_in);
}

/**
 * error_and_update_ex - Prints an error message
 * 						 and updates the command exit status.
 *
 * @cmd: Pointer to the command structure to update the exit status.
 * @file: Name of the file that caused the error
 * 		  (printed as part of the error message).
 * @error: Description of the error to print.
 * @ex: Exit status code to assign to the command.
 *
 * Formats and prints an error message of the form
 * "<file>: <error>" to standard error.
 * Updates the `cmd_exit` field of the command structure
 * to the specified exit code.
 */
void	error_and_update_ex(t_cmd *cmd, char *file, char *error, int ex)
{
	if (file && file[0] != '\0')
		ft_putstr_fd(file, 2);
	write(2, ": ", 2);
	ft_putendl_fd(error, 2);
	cmd->cmd_exit = ex;
}
