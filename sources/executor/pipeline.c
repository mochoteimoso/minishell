/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/14 15:44:29 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		execute_pipeline(t_shell *mini);
int		initialize_pipeline(int pipe_fds[][2], int count);
int		create_pipes(int pipe_fds[][2], int count);
//int		resolve_all_fds(t_shell *mini, int count);
int		fork_and_execute(t_shell *mini, int pipe_fds[][2], int count);
int		setup_and_execute(t_shell *mini, int pipe_fds[][2], int count, int i);
int		dup2_and_close(int old_fd, int new_fd);
void	close_pipe_fds(int pipe_fds[][2], int count);
void	execute_cmd(t_shell *mini, t_cmd *cmd);
int		execute_single_cmd(t_shell *mini);

int	execute_pipeline(t_shell *mini)
{
	int		count = 0;
	int		pipe_fds[count][2];
	int		i;

	i = 0;
	count = mini->pipe_count;
	if (count == 0)
	{
		execute_single_cmd(mini);
		return (0);
	}
	if (initialize_pipeline(pipe_fds, count) == -1)
		return (1);
	if (fork_and_execute(mini, pipe_fds, count) == -1)
	{
		close_pipe_fds(pipe_fds, count);
		return (1);
	}
	close_pipe_fds(pipe_fds, count);
	
	// Wait all children to finish executing
	while (i < count)
	{
		wait(NULL);
		i++;
	}
	return (0);
}

int	execute_single_cmd(t_shell *mini)
{
	t_cmd	*cmd;
	
	cmd = mini->cmds[0];
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2_and_close(cmd->fd_in, STDIN_FILENO))
			return (1);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2_and_close(cmd->fd_out, STDOUT_FILENO))
			return (1);
	}
	execute_cmd(mini, mini->cmds[0]);
	return (0);
	/*int	i = 0;
	while (env_array[i])
	{
		printf("%s\n", env_array[i]);
		i++;
	} */
} 

// Creates pipes for the pipeline

int	initialize_pipeline(int pipe_fds[][2], int count)
{
	if (create_pipes(pipe_fds, count) == -1)
		return (-1);
	//if (resolve_all_fds(mini, count) == -1)
	//	return (-1);
	return (0);
}

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

/*int	resolve_all_fds(t_shell *mini, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (resolve_fd(mini->cmds[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
} */

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
		{
			if (setup_and_execute(mini, pipe_fds, count, i))
				return (-1);
		}
		i++;
	}
	return (0);
}

// Handles pipe and redirection setup for each child

int	setup_and_execute(t_shell *mini, int pipe_fds[][2], int count, int i)
{
	t_cmd *cmd;
	char **env_array;
	
	cmd = mini->cmds[i];
	env_array = env_to_array(mini->env);
	// Input redirection
	printf("Duplicating first input fd\n");
	if (cmd->fd_in != -1)
	{
		if (dup2_and_close(cmd->fd_in, STDIN_FILENO))
			return (1);
		printf("First input fd ok\n");
	}
	else if (i > 0)
	{
		if (dup2_and_close(pipe_fds[i - 1][0], STDIN_FILENO))
			return (1);
		printf("Next input fd ok\n");
	}
	// Output redirection
	if (cmd->fd_out != -1)
	{
		if (dup2_and_close(cmd->fd_out, STDOUT_FILENO))
			return (1);
		printf("First output fd ok\n");
	}
	else if (i < count)
	{
		if (dup2_and_close(pipe_fds[i][1], STDOUT_FILENO))
			return (1);
		printf("Next output fd ok\n");
	}
	close(cmd->fd_in);
	close(cmd->fd_out);
	close_pipe_fds(pipe_fds, count);
	execute_cmd(mini, cmd);
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
	char	**env_array;
	
	env_array = env_to_array(mini->env);
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
	{
		perror(cmd->command);
		// free everything
		exit(EXIT_FAILURE);
	}
}

