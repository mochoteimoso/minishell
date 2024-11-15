/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/15 10:02:50 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		execute_pipeline(t_shell *mini, char **envp);
int		execute_single_cmd(t_shell *mini, char **envp);
int		fork_and_execute(t_shell *mini, int pipe_fds[][2], int count, char **envp);
int		setup_and_execute(t_shell *mini, int pipe_fds[][2], int count, int i, char **envp);
int		execute_cmd(t_cmd *cmd, char **envp);

// Initializes an array of pipe_fds based on the number of pipes
// executes single command if there are no pipes
// sets up pipeline and forks child processes

int	execute_pipeline(t_shell *mini, char **envp)
{
	int		count = 0;
	int		pipe_fds[count][2];
	int		i;

	i = 0;
	count = mini->pipe_count;
	if (count == 0)
	{
		if (execute_single_cmd(mini, envp))
			return (1);
	}
	if (create_pipes(pipe_fds, count) == -1)
		return (1);
	if (fork_and_execute(mini, pipe_fds, count, envp) == -1)
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

// Executes single command if there are no pipes
// duplicates fd based on fd_in and fd_out

int	execute_single_cmd(t_shell *mini, char **envp)
{
	if (mini->cmds[0]->fd_in != STDIN_FILENO)
	{
		if (dup2_and_close(mini->cmds[0]->fd_in, STDIN_FILENO))
			return (1);
	}
	if (mini->cmds[0]->fd_out != STDOUT_FILENO)
	{
		if (dup2_and_close(mini->cmds[0]->fd_out, STDOUT_FILENO))
			return (1);
	}
	if (execute_cmd(mini->cmds[0], envp))
		return (1);
	return (0);
	/*int	i = 0;
	while (env_array[i])
	{
		printf("%s\n", env_array[i]);
		i++;
	} */
} 

int	fork_and_execute(t_shell *mini, int pipe_fds[][2], int count, char **envp)
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
			if (setup_and_execute(mini, pipe_fds, count, i, envp))
				return (-1);
		}
		i++;
	}
	return (0);
}

// Handles pipe and redirection setup for each child

int	setup_and_execute(t_shell *mini, int pipe_fds[][2], int count, int i, char **envp)
{
	t_cmd *cmd;
	//char **env_array;
	
	cmd = mini->cmds[i];
	//env_array = env_to_array(mini->env);
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
	execute_cmd(cmd, envp);
	return (0);
}


// Executes command
// Check why env_array parsed based on min->env is not working

int	execute_cmd(t_cmd *cmd, char **envp)
{
	//char	**env_array;
	
	//env_array = env_to_array(mini->env);
	if (execve(cmd->cmd_path, cmd->args, envp) == -1)
	{
		perror(cmd->command);
		// free everything
		return (1);
	}
	return (0);
}

