/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:18:57 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/19 20:33:53 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		open_input_file(t_cmd *cmd, char *input_file);
int		open_output_file(t_cmd *cmd, char *output_file);
int		open_append_file(t_cmd *cmd, char *output_file);
int		open_heredoc(t_cmd *cmd, char *delimiter);
void	error_and_update_ex(t_cmd * cmd, char *file, char *error, int ex);

// Tries to open input file and prints correct error in case of failure

int	open_input_file(t_cmd *cmd, char *input_file)
{
	int	fd_in;

	fd_in = open(input_file, O_RDONLY);
	if (fd_in == -1)
	{
		if (access(input_file, F_OK) != 0)
		{
			error_and_update_ex(cmd, input_file, "No such file or directory", 1);
			return (-2);
		}
		if (access(input_file, R_OK) != 0)
		{
			ft_putstr_fd(input_file, 2);
			error_and_update_ex(cmd, input_file, "Permission denied", 126);
			return (-2);
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
		error_and_update_ex(cmd, output_file, "Is a directory", 1);
		return (-2);
	}
	if (access(output_file, F_OK) == 0 && access(output_file, W_OK) == -1)
	{
		error_and_update_ex(cmd, output_file, "Permission denied", 1);
		return (-2);
	}
	fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		error_and_update_ex(cmd, output_file, "No such file or directory", 1);
		return (-2);
	}
	return (fd_out);
}

// Tries to open output append file and prints correct error in case of failure

int	open_append_file(t_cmd *cmd, char *output_file)
{
	int	fd_out;
	
	fd_out = open(output_file, O_DIRECTORY);
	if (fd_out != -1)
	{
		close(fd_out);
		error_and_update_ex(cmd, output_file, "Is a directory", 1);
		return (-2);
	}
	if (access(output_file, F_OK) == 0 && access(output_file, W_OK) == -1)
	{
		error_and_update_ex(cmd, output_file, "Permission denied", 1);
		return (-2);
	}
	fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		error_and_update_ex(cmd, output_file, "No such file or directory", 1);
		return (-2);
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
			error_and_update_ex(cmd, heredoc_file, "No such file or directory", 1);
			return (-2);
		}
		if (access(heredoc_file, R_OK) != 0)
		{
			error_and_update_ex(cmd, heredoc_file, "Permission denied", 1);
			return (-2);
		}
	}
	return (fd_in);
}

void	error_and_update_ex(t_cmd * cmd, char *file, char *error, int ex)
{
	if (file && file[0] != '\0')
		ft_putstr_fd(file, 2);
	write(2, ": ", 2);
	ft_putendl_fd(error, 2);
	cmd->cmd_exit = ex;
}
