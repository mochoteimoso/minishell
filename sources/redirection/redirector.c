/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:10:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/14 13:21:51 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	resolve_fd(t_cmd *cmd);
int resolve_input_fd(t_cmd *cmd);
int resolve_output_fd(t_cmd *cmd);
int	open_input_file(char *input_file);
int	open_output_file(char *output_file);
int	open_append_file(char *output_file);
int open_heredoc(char *delimiter);

// Figures out fds for input and output

int	resolve_fd(t_cmd *cmd)
{
	int	fd_in;
	int	fd_out;
	
	fd_in = -1;
	fd_out = -1;	
	fd_in = resolve_input_fd(cmd);
	//if (fd_in == -1 && cmd->redir_head)
	//	return (EXIT_FAILURE);
	if (fd_in != -1)
		cmd->fd_in = fd_in;
	else
		cmd->fd_in = STDIN_FILENO;
	fd_out = resolve_output_fd(cmd);
	//if (fd_out == -1 && cmd->redir_head)
	//{disp
	//	if (fd_in != -1)
	//		close(fd_in);
	//	return (EXIT_FAILURE);
	//}
	if (fd_out != -1)
		cmd->fd_out = fd_out;
	else
		cmd->fd_out = STDOUT_FILENO;
	return (0);
}

int	resolve_input_fd(t_cmd *cmd)
{
	int		fd_in;
	t_redir	*temp;

	fd_in = -1;
	temp = cmd->redir_head;
	while (temp)
	{
		if (temp->type == 0 || temp->type == 3)
		{
			if (fd_in != -1)
				close(fd_in);
			if (temp->type == 0)
				fd_in = open_input_file(temp->file);
			else if (temp->type == 3)
				fd_in = open_heredoc(temp->delimiter);
			if (fd_in == -1)
				return (-1);
		}
		temp = temp->next;
	}
	return (fd_in);
}

int	resolve_output_fd(t_cmd *cmd)
{
	int		fd_out;
	t_redir	*temp;

	fd_out = -1;
	temp = cmd->redir_head;
	while (temp)
	{
		if (temp->type == 1 || temp->type == 2)
		{
			if (fd_out != -1)
				close(fd_out);
			if (temp->type == 1)
				fd_out = open_output_file(temp->file);
			else if (temp->type == 2)
				fd_out = open_append_file(temp->file);
			if (fd_out == -1)
				return (-1);
		}
		temp = temp->next;
	}
	return (fd_out);
}

// Gets fd for input file
/*
int resolve_input_fd(t_cmd *cmd)
{
	int		fd_in;
	t_redir	*temp;

	fd_in = -1;
	temp = cmd->redir_head;
	while (temp && (temp->type == 0 || temp->type == 3))
	{
		if (fd_in != -1)
			close(fd_in);
		if (temp->type == 0)
			fd_in = open_input_file(temp->file);
		else if (temp->type == 3)
			fd_in = open_heredoc(temp->delimiter);
		if (fd_in == -1)
			return (-1);
		temp = temp->next;
	}
	return (fd_in);
}

// Gets fd for output file

int resolve_output_fd(t_cmd *cmd)
{
	int		fd_out;
	t_redir	*temp;

	fd_out = -1;
	temp = cmd->redir_head;
	while (temp && (temp->type == 1 || temp->type == 2))
	{
		if (fd_out != -1)
			close(fd_out);
		if (temp->type == 1)
			fd_out = open_output_file(temp->file);
		else if (temp->type == 2)
			fd_out = open_append_file(temp->file);
		if (fd_out == -1)
			return (-1);
		temp = temp->next;
	}
	return (fd_out);
} */

// Tries to open input file and prints correct error in case of failure

int	open_input_file(char *input_file)
{
	int	fd_in;

	fd_in = open(input_file, O_RDONLY);
	if (fd_in == -1)
	{
		if (access(input_file, F_OK) != 0)
		{
			ft_putstr_fd(input_file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
		else
		{
			ft_putstr_fd(input_file, 2);
			ft_putstr_fd(": Permission denied\n", 2);
		}
		return(-1);
	}
	return (fd_in);
}

// Tries to open output file and prints correct error in case of failure

int	open_output_file(char *output_file)
{
	int	fd_out;

	fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		if (access(output_file, F_OK))
		{
			ft_putstr_fd(output_file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
		else
		{
			ft_putstr_fd(output_file, 2);
			ft_putstr_fd(": Permission denied\n", 2);
		}
		return(-1);
	}
	return (fd_out);
}

// Tries to open output append file and prints correct error in case of failure

int	open_append_file(char *output_file)
{
	int	fd_out;

	fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		if (access(output_file, F_OK))
		{
			ft_putstr_fd(output_file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
		else
		{
			ft_putstr_fd(output_file, 2);
			ft_putstr_fd(": Permission denied\n", 2);
		}
		return(-1);
	}
	return (fd_out);
}

// Opens heredoc

int	open_heredoc(char *delimiter) 
{
	int		heredoc_pipe_fd[2];
	char	*line;

	if (pipe(heredoc_pipe_fd) == -1)
	{
		perror("pipe error");
		return (-1);
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
}



