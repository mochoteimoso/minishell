/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/18 16:11:06 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		execute_pipeline(t_shell *mini);
int		execute_single_cmd(t_shell *mini, char **envp);
int		fork_and_execute(t_shell *mini, t_cmd *cmd, int pipe_fd[2], char **envp, int i);
int		execute_cmd(t_cmd *cmd, char **envp);
void	setup_fds(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i);
void	close_pipes(t_shell *mini, int pipe_fd[2]);

// Initializes an array of pipe_fds based on the number of pipes
// executes a single command if there are no pipes
// sets up pipeline and forks child processes

int	execute_pipeline(t_shell *mini)
{
	int		pipe_fd[2];
	int		i;
	t_cmd	*cmd;
	char	**envp;

	i = 0;
	envp = env_to_array(mini->env);
	if (mini->cmd_count == 1)
	{
		if (execute_single_cmd(mini, envp))
			return (1);
	}
	while (i < mini->cmd_count)
	{
		cmd = mini->cmds[i];
		if (i < mini->cmd_count - 1 && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
		setup_fds(mini, cmd, pipe_fd, i);
		if (fork_and_execute(mini, cmd, pipe_fd, envp, i) == -1)
			return (1);
		close_pipes(mini, pipe_fd);
		mini->prev_pipe[0] = pipe_fd[0];
		mini->prev_pipe[1] = -1;  // Close the previous output for the next command
		i++;
	}
	// Wait for all children to finish
	while (wait(NULL) > 0);
	return (0);
}

void	setup_fds(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i)
{
	if (i != 0 && cmd->fd_in == STDIN_FILENO)
		cmd->fd_in = mini->prev_pipe[0]; // get input from previous command
	if (i < mini->cmd_count - 1 && cmd->fd_out == STDOUT_FILENO)
		cmd->fd_out = pipe_fd[1]; // Output to the next pipe
}

int	fork_and_execute(t_shell *mini, t_cmd *cmd, int pipe_fd[2], char **envp, int i)
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
		if (dup_input(mini, cmd, i)) // redirect input
			return (1);
		if (dup_output(cmd, pipe_fd, mini->cmd_count, i)) //redirect output
			return (1);
		execute_cmd(cmd, envp);
		return (0);
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

