/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:04:04 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/09 18:26:43 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void print_heredoc_stuff(int fd)
{
	char	buffer[1024];
	ssize_t	n;

	while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[n] = '\0';
		printf("%s", buffer);
	}
}

void	heredoc(char *input)
{
	int		pipe_fd[2];
	char	*line;
	char	**cmd;
	char	*delimiter;

	cmd = ft_split(input, ' ');
	delimiter = cmd[1];
	if (pipe(pipe_fd) == -1)
	{
		error("No pipe\n");
		return ;
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	print_heredoc_stuff(pipe_fd[0]);
	close(pipe_fd[0]);
}
