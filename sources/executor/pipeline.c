/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/18 11:37:00 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		execute_pipeline(t_shell *mini, char **envp);
int		execute_single_cmd(t_shell *mini, char **envp);
int		fork_and_execute(t_cmd *cmd, char **envp);
//int	setup_and_execute(t_shell *mini, int pipe_fd[2], int count, int i, char **envp);
int	execute_cmd(t_cmd *cmd, char **envp);

int	execute_pipeline(t_shell *mini, char **envp) 
{
	int		pipe_fd[2];
	int		prev_in;
	int		i;
	t_cmd	*cmd;
	
	prev_in = STDIN_FILENO;
	i = 0;
	while (i < mini->cmd_count)
	{
		cmd = mini->cmds[i];
		if (i < mini->cmd_count - 1 && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
		// Set up the command's file descriptors
		if (cmd->fd_in == STDIN_FILENO)
			cmd->fd_in = prev_in;
		if (i < mini->cmd_count - 1)
			cmd->fd_out = pipe_fd[1];
		// Fork and execute
		if (fork_and_execute(cmd, envp) == -1)
			return (1);
		// Close parent's ends of the current pipe
		if (prev_in != STDIN_FILENO)
			close(prev_in);
		if (i < mini->cmd_count - 1)
			close(pipe_fd[1]);
		// Update prev_in for the next command
		prev_in = pipe_fd[0];
		i++;
	}
	// Wait for all children to finish
	for (int i = 0; i < mini->cmd_count; i++) wait(NULL);
	return (0);
}


int	fork_and_execute(t_cmd *cmd, char **envp) 
{
	pid_t	pid;
	
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
	{
		// Redirect input
		if (cmd->fd_in != STDIN_FILENO) 
		{
			if (dup2(cmd->fd_in, STDIN_FILENO) == -1) 
			{
				perror("dup2 input");
				exit(EXIT_FAILURE);
			}
			close(cmd->fd_in);
		}
		// Redirect output
		if (cmd->fd_out != STDOUT_FILENO) {
			if (dup2(cmd->fd_out, STDOUT_FILENO) == -1) {
				perror("dup2 output");
				exit(EXIT_FAILURE);
			}
			close(cmd->fd_out);
		}
		// Execute the command
		execve(cmd->cmd_path, cmd->args, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	return (0);
}

// Initializes an array of pipe_fds based on the number of pipes
// executes single command if there are no pipes
// sets up pipeline and forks child processes

/*int	execute_pipeline(t_shell *mini, char **envp)
{
	int		i;
	int		count;

	i = 0;
	count = mini->cmd_count;
	mini->prev_pipe[0] = -1;
	mini->prev_pipe[1] = -1;
	if (count == 1)
	{
		if (execute_single_cmd(mini, envp))
			return (1);
	}
	while (i < count)
	{
		if (fork_and_execute(mini, count, i, envp))
			return (1);
		i++;
	}
	// Wait all children to finish executing
	while (i < count)
	{
		wait(NULL);
		i++;
	}
	return (0);
} */

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
} 
/*
// Fork a child process for each command

pid_t	fork_and_execute(t_shell *mini, int count, int i, char **envp)
{
	pid_t	pid;
	int		pipe_fd[2] = {-1, -1};
	
	if (i < count - 1)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
		printf("Pipe created: pipe_fd[0] = %d, pipe_fd[1] = %d\n", pipe_fd[0], pipe_fd[1]);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)  // child process
	{
		if (setup_and_execute(mini, pipe_fd, count, i, envp))
			exit(EXIT_FAILURE);
		return (0);
	}
	if (i > 0)  // If it's not the first command, close the read end of the previous pipe
		close(pipe_fd[0]);
	if (i < count - 1)  // If it's not the last command, close the write end of the current pipe
		close(pipe_fd[1]);
	mini->prev_pipe[0] = pipe_fd[0];
	mini->prev_pipe[1] = pipe_fd[1];
	return (pid);
}

// Handles pipe and redirection setup for each child

int	setup_and_execute(t_shell *mini, int pipe_fd[2], int count, int i, char **envp)
{
	t_cmd *cmd;
	//char **env_array;
	
	cmd = mini->cmds[i];
	//env_array = env_to_array(mini->env);
	if (dup_input(mini, cmd, i))
		return (1);
	if (dup_output(cmd, pipe_fd, count, i))
		return (1);
	//close(mini->cmds[i]->fd_out);
	close_pipe_fds(pipe_fd);
	execute_cmd(cmd, envp);
	return (0);
}

*/

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
	return (EXIT_SUCCESS);
}

