/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/13 16:04:56 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		execute_pipeline(t_shell *mini);
int		create_pipes(int pipe_fds[][2], int count);
int		resolve_all_fds(t_shell *mini, int count);
int		fork_and_execute(t_shell *mini, int pipe_fds[][2], int count);
int		set_pipes_and_execute(t_shell *mini, int pipe_fds[][2], int count, int i);
void 	dup2_and_close(int old_fd, int new_fd);
void	close_pipe_fds(int pipe_fds[][2], int count);
void	execute_cmd(t_shell *mini, t_cmd *cmd);

int	execute_pipeline(t_shell *mini)
{
	int	count = 0;
	int	pipe_fds[count - 1][2];
	int	i;

	i = 0;
	count = mini->pipe_count;
	if (create_pipes(pipe_fds, count -1) == -1)
		return (-1);
	if (resolve_all_fds(mini, count) == -1)
	{
		close_pipe_fds(pipe_fds, count - 1);
		return (-1);
	}
	if (fork_and_execute(mini, pipe_fds, count) == -1)
	{
		close_pipe_fds(pipe_fds, count - 1);
		return (-1);
	}
	close_pipe_fds(pipe_fds, count - 1);
	
	// Wait all children to finish executing
	while (i < count)
	{
		wait(NULL);
		i++;
	}
	return (0);
}

// Creates pipes for the pipeline

int	create_pipes(int pipe_fds[][2], int count)
{
	int	i;

	i = 0;
	while (i < count)
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

// Finds fds for all commands in the pipeline

int	resolve_all_fds(t_shell *mini, int count)
{
	int	i;

	i = 0;
	while(i < count)
	{
		if (resolve_fd(mini->cmds[i]) == -1)
			return (-1);
	}
	return (0);
}

// Forks and executes each command in the pipeline

int	fork_and_execute(t_shell *mini, int pipe_fds[][2], int count)
{
	int		i;
	pid_t	pid;
	
	i = 0;
	while (i < count)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (-1);
		}
		else if (pid == 0)
			set_pipes_and_execute(mini, pipe_fds, count, i);
		i++;
	}
	return (0);
}

// Handles pipe and redirection setup for each child

int	set_pipes_and_execute(t_shell *mini, int pipe_fds[][2], int count, int i)
{
	t_cmd *cmd;
	
	cmd = mini->cmds[i];
	
	// Input redirection
	if (cmd->fd_in != -1)
		dup2_and_close(cmd->fd_in, STDIN_FILENO);
	else if (i > 0) 
		dup2_and_close(pipe_fds[i - 1][0], STDIN_FILENO);

	// Output redirection
	if (cmd->fd_out != -1)
		dup2_and_close(cmd->fd_out, STDOUT_FILENO);
	else if (i < count - 1)
		dup2_and_close(pipe_fds[i][1], STDOUT_FILENO);
	close_pipe_fds(pipe_fds, count - 1);
	execute_cmd(mini, cmd);
	exit(EXIT_FAILURE);
}

// Helper function to duplicate and close fd

void dup2_and_close(int old_fd, int new_fd)
{
	dup2(old_fd, new_fd);
	close(old_fd);
}

// Close all pipes in the array

void	close_pipe_fds(int pipe_fds[][2], int count)
{
	int	i;
	
	i = 0;
	while (i < count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		i++;
	}
}

void	execute_cmd(t_shell *mini, t_cmd *cmd)
{
	if (execve(cmd->cmd_path, cmd->args, mini->pending) == -1)
	{
		perror(cmd->command);
		// free everything
		exit(EXIT_FAILURE);
	}
}

