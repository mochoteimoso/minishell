/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:18:57 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 15:18:19 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		open_input_file(t_cmd *cmd, char *input_file);
int		open_output_file(t_cmd *cmd, char *output_file);
int		open_append_file(t_cmd *cmd, char *output_file);
int		open_heredoc(t_cmd *cmd, char *delimiter);
void	error_and_update(t_cmd *cmd, char *file, char *error, int ex);

// Tries to open input file and prints correct error in case of failure

int	open_input_file(t_cmd *cmd, char *file)
{
	int	fd_in;

	fd_in = open(file, O_RDONLY);
	if (fd_in == -1)
	{
		if (access(file, F_OK) != 0)
		{
			error_and_update(cmd, file, "No such file or directory", 1);
			return (-1);
		}
		if (access(file, R_OK) != 0)
		{
			ft_putstr_fd(file, 2);
			error_and_update(cmd, file, "Permission denied", 126);
			return (-1);
		}
	}
	return (fd_in);
}

// Tries to open output file and prints correct error in case of failure

int	open_output_file(t_cmd *cmd, char *output_file)
{
	int	fd_out;

	fd_out = open(output_file, O_DIRECTORY);
	if (fd_out != -1)
	{
		close(fd_out);
		error_and_update(cmd, output_file, "Is a directory", 1);
		return (-1);
	}
	if (access(output_file, F_OK) == 0 && access(output_file, W_OK) == -1)
	{
		error_and_update(cmd, output_file, "Permission denied", 1);
		return (-2);
	}
	fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		error_and_update(cmd, output_file, "No such file or directory", 1);
		return (-1);
	}
	return (fd_out);
}

// Tries to open output append file and prints correct error in case of failure

int	open_append_file(t_cmd *cmd, char *file)
{
	int	fd_out;

	fd_out = open(file, O_DIRECTORY);
	if (fd_out != -1)
	{
		close(fd_out);
		error_and_update(cmd, file, "Is a directory", 1);
		return (-1);
	}
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
	{
		error_and_update(cmd, file, "Permission denied", 1);
		return (-1);
	}
	fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		error_and_update(cmd, file, "No such file or directory", 1);
		return (-1);
	}
	return (fd_out);
}

// Opens heredoc

int	open_heredoc(t_cmd *cmd, char *heredoc_file)
{
	int	fd_in;

	fd_in = open(heredoc_file, O_RDONLY);
	if (fd_in == -1)
	{
		if (access(heredoc_file, F_OK) != 0)
		{
			error_and_update(cmd, heredoc_file, "No such file or directory", 1);
			return (-1);
		}
		if (access(heredoc_file, R_OK) != 0)
		{
			error_and_update(cmd, heredoc_file, "Permission denied", 1);
			return (-1);
		}
	}
	return (fd_in);
}

void	error_and_update(t_cmd *cmd, char *file, char *error, int ex)
{
	if (file && file[0] != '\0')
		ft_putstr_fd(file, 2);
	write(2, ": ", 2);
	ft_putendl_fd(error, 2);
	cmd->cmd_exit = ex;
}
