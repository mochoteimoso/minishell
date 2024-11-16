/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/16 18:30:30 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		execute_pipeline(t_shell *mini, char **envp);
int		execute_single_cmd(t_shell *mini, char **envp);
pid_t	fork_and_execute(t_shell *mini, int count, int i, char **envp);
int	setup_and_execute(t_shell *mini, int pipe_fd[2], int count, int i, char **envp);
int	execute_cmd(t_cmd *cmd, char **envp);

// Initializes an array of pipe_fds based on the number of pipes
// executes single command if there are no pipes
// sets up pipeline and forks child processes

int	execute_pipeline(t_shell *mini, char **envp)
{
	int		i;
	int		count;

	i = 0;
	count = mini->cmd_count;
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
} 

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
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)  // child process
	{
		if (setup_and_execute(mini, pipe_fd, count, i, envp))
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	/*else
	{
		if (i > 0)
			close(pipe_fd[0]);
		if (i < count - 1)
			close(pipe_fd[1]);
	} */
	return (pid);
}

// Handles pipe and redirection setup for each child

int	setup_and_execute(t_shell *mini, int pipe_fd[2], int count, int i, char **envp)
{
	t_cmd *cmd;
	//char **env_array;
	
	cmd = mini->cmds[i];
	//env_array = env_to_array(mini->env);
	if (dup_input(cmd, pipe_fd, i))
		return (1);
	if (dup_output(cmd, pipe_fd, count, i))
		return (1);
	close(mini->cmds[i]->fd_out);
	close_pipe_fds(pipe_fd);
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
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

