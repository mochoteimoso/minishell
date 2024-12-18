/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:18:57 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/16 11:33:55 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_input_file(t_cmd *cmd, char *input_file);
int	open_output_file(t_cmd *cmd, char *output_file);
int	open_append_file(t_cmd *cmd, char *output_file);
int	open_heredoc(t_cmd *cmd, char *delimiter);

// Tries to open input file and prints correct error in case of failure

int	open_input_file(t_cmd *cmd, char *input_file)
{
	int	fd_in;

	fd_in = open(input_file, O_RDONLY);
	if (fd_in == -1)
	{
		if (access(input_file, F_OK) != 0)
		{
			ft_putstr_fd(input_file, 2);
			ft_putendl_fd(": No such file or directory", 2);
		}
		else
		{
			ft_putstr_fd(input_file, 2);
			ft_putendl_fd(": Permission denied", 2);
		}
		cmd->cmd_exit = 1;
		return (-2);
	}
	return (fd_in);
}

// Tries to open output file and prints correct error in case of failure

int	open_output_file(t_cmd *cmd, char *output_file)
{
	int	fd_out;

	fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		if (access(output_file, F_OK))
		{
			ft_putstr_fd(output_file, 2);
			ft_putendl_fd(": No such file or directory", 2);
		}
		else
		{
			ft_putstr_fd(output_file, 2);
			ft_putendl_fd(": Permission denied", 2);
		}
		cmd->cmd_exit = 1;
		return (-2);
	}
	return (fd_out);
}

// Tries to open output append file and prints correct error in case of failure

int	open_append_file(t_cmd *cmd, char *output_file)
{
	int	fd_out;

	fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		if (access(output_file, F_OK))
		{
			ft_putstr_fd(output_file, 2);
			ft_putendl_fd(": No such file or directory", 2);
		}
		else
		{
			ft_putstr_fd(output_file, 2);
			ft_putendl_fd(": Permission denied", 2);
		}
		cmd->cmd_exit = 1;
		return (-2);
	}
	return (fd_out);
}

// Opens heredoc

/*int	open_heredoc(t_cmd *cmd, char *delimiter)
{
	int		heredoc_pipe_fd[2];
	char	*line;

	if (pipe(heredoc_pipe_fd) == -1)
	{
		perror("pipe error");
		cmd->cmd_exit = 1;
		return (-2);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(heredoc_pipe_fd[1], line, ft_strlen(line));
		write(heredoc_pipe_fd[1], "\n", 1);
		free(line);
	}
	close(heredoc_pipe_fd[1]);
	return (heredoc_pipe_fd[0]);
} */

int	open_heredoc(t_cmd *cmd, char *heredoc_file)
{
	int fd_in;
	fd_in = open(heredoc_file, O_RDONLY);
	if (fd_in == -1)
	{
		if (access(heredoc_file, F_OK) != 0)
		{
			ft_putstr_fd(heredoc_file, 2);
			ft_putendl_fd(": No such file or directory", 2);
		}
		else
		{
			ft_putstr_fd(heredoc_file, 2);
			ft_putendl_fd(": Permission denied", 2);
		}
		cmd->cmd_exit = 1;
		return (-2);
	}
	return (fd_in);
}
