/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/16 14:51:03 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_pipeline(t_shell *mini, char **envp);
int	execute_single_cmd(t_shell *mini, char **envp);
int	fork_and_execute(t_shell *mini, int pipe_fds[][2], int count, char **envp);
int	setup_and_execute(t_shell *mini, int pipe_fds[][2], int count, int i, char **envp);
int	dup_input(t_cmd *cmd, int pipe_fds[][2], int i);
int	dup_output(t_cmd *cmd, int pipe_fds[][2], int count, int i);
int	execute_cmd(t_cmd *cmd, char **envp);

// Initializes an array of pipe_fds based on the number of pipes
// executes single command if there are no pipes
// sets up pipeline and forks child processes

int	execute_pipeline(t_shell *mini, char **envp)
{
	int		count = mini->cmd_count;
	int		pipe_fds[count - 1][2];
	int		i;

	i = 0;
	//count = mini->cmd_count;
	if (count == 0)
	{
		if (execute_single_cmd(mini, envp))
			return (1);
	}
	if (create_pipes(pipe_fds, count) == -1)
		return (1);
	if (fork_and_execute(mini, pipe_fds, count, envp))
	{
		close_pipe_fds(pipe_fds, count);
		return (1);
	}
	//close_pipe_fds(pipe_fds, count);
	
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

// Fork a child process for each command

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
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)  // child process
		{
			if (setup_and_execute(mini, pipe_fds, count, i, envp))
				exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		if (i < count - 1)
		{
			close(pipe_fds[i][1]);
			printf("Parent closed pipe_fds[%d][1]: %d\n", i, pipe_fds[i][1]);
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
	if (dup_input(cmd, pipe_fds, i))
		return (1);
	if (dup_output(cmd, pipe_fds, count, i))
		return (1);
	//close(cmd->fd_in); // not required in child process?
	//close(cmd->fd_out); // not required in child process?
	close_pipe_fds(pipe_fds, count);
	execute_cmd(cmd, envp);
	return (0);
}

// Duplicates input from fd or pipe_fd

int	dup_input(t_cmd *cmd, int pipe_fds[][2], int i)
{
	//printf("Value of i in input: %d\n", i);
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2_and_close(cmd->fd_in, STDIN_FILENO))
		{
			perror("dup2 for fd_out");
			return (1);
		}
		//printf("Input redirection handled for cmd[%d].\n", i);
	}
	else if (i > 0)  // Use pipe for input
	{
		if (dup2_and_close(pipe_fds[i - 1][0], STDIN_FILENO))
		{
			perror("dup2 for pipe output");
			return (1);
		}
		 //printf("Pipe input handled for cmd[%d].\n", i);
	}
	return (0);
}

// Duplicates output to fd or pipe_fd

int	dup_output(t_cmd *cmd, int pipe_fds[][2], int count, int i)
{
	//printf("Value of i in output: %d\n", i);
	//printf("Value of count: %d\n", count);
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2_and_close(cmd->fd_out, STDOUT_FILENO))
			return (1);
		//printf("Output redirection handled for cmd[%d].\n", i);
	}
	if (i < count - 1) // Use pipe for output
	{
		if (dup2_and_close(pipe_fds[i][1], STDOUT_FILENO))
			return (1);
		//printf("Pipe output handled for cmd[%d].\n", i);
	}
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
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

