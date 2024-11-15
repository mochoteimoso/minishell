/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:58:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/15 19:05:38 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Creates pipes for the pipeline

int	create_pipes(int pipe_fds[][2], int cmd_count)
{
	int	i;
	int	pipe_count;

	i = 0;
	pipe_count = cmd_count - 1;
	while (i < pipe_count)
	{
		if (pipe(pipe_fds[i]) == -1)
		{
			perror("pipe");
			return (-1);
		}
		i++;
	}
	return (0);
}

// Helper function to duplicate and close fd

int	dup2_and_close(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		close(old_fd);
		perror("dup2");
		return (1);
	}
	close(old_fd);
	return (0);
}

// Close all pipes in the array

void	close_pipe_fds(int pipe_fds[][2], int cmd_count)
{
	int	i;
	
	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		i++;
	}
}
