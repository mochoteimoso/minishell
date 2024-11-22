/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/21 10:26:54 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		execute_pipeline(t_shell *mini);
int		handle_single_cmd(t_shell *mini);
int		fork_and_execute(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i);
int		execute_cmd(t_shell *mini, t_cmd *cmd);
void	setup_fds(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i);
void	close_pipes(t_shell *mini, int pipe_fd[2]);
void	wait_children(t_shell *mini);

// Initializes an array of pipe_fds based on the number of pipes
// executes a single command if there are no pipes
// sets up pipeline and forks child processes

int	execute_pipeline(t_shell *mini)
{
	int		pipe_fd[2];
	int		i;
	t_cmd	*cmd;

	i = 0;
	if (mini->cmd_count == 1 && is_this_built(mini->cmds[0]->command))
	{
		if (handle_single_cmd(mini))
			return (1);
		clean_cmds(mini->cmds);
		return (0);
	}
	mini->pids = ft_calloc(mini->cmd_count, sizeof(pid_t));
	if (!mini->pids)
	{
		clean_cmds(mini->cmds);   // create a function that also sets pointer to null!
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
		if (fork_and_execute(mini, cmd, pipe_fd, i) == -1)
			return (1);
		if (mini->cmd_count > 1)
		{
			close_pipes(mini, pipe_fd);
			mini->prev_pipe[0] = pipe_fd[0];
			mini->prev_pipe[1] = -1;  // Closes the previous output for the next command
		}
		i++;
	}
	/* print child process PIDS for debugging purposes - remove!
	int j = 0;
	while (j < mini->cmd_count)
	{
		printf("PID %d: %d\n", j, mini->pids[j]);
		j++;
	}
	*/
	clean_cmds(mini->cmds);
	wait_children(mini);
	return (0);
}

void	wait_children(t_shell *mini)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < mini->cmd_count)
	{
		waitpid(mini->pids[i], &status, 0);
		if (WIFEXITED(status))
			mini->exit_stat = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			printf("Child %d terminated by signal %d\n", mini->pids[i], WTERMSIG(status));
		i++;
	}
}

void	setup_fds(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i)
{
	if (i != 0 && cmd->fd_in == STDIN_FILENO)
		cmd->fd_in = mini->prev_pipe[0]; // get input from previous command
	if (i < mini->cmd_count - 1 && cmd->fd_out == STDOUT_FILENO)
		cmd->fd_out = pipe_fd[1]; // Output to the next pipe
}

int	fork_and_execute(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i)
{

	// add save_fds and reset_fds here somewhere

	sig_handler_changer();
	mini->pids[i] = fork();
	if (mini->pids[i] == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (mini->pids[i] == 0)
	{
		if (dup_input(mini, cmd, i)) // redirect input
			return (1);
		if (dup_output(cmd, pipe_fd, mini->cmd_count, i)) //redirect output
			return (1);
		if (is_this_built(cmd->command))
		{
			if (built_in_exe(mini, cmd))
				exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		else
			execute_cmd(mini, cmd);
	}
	return (0);
}

// Executes single command if there are no pipes
// duplicates fd based on fd_in and fd_out

int	handle_single_cmd(t_shell *mini)
{
	if (save_fds(mini))
		return (1);
	if (mini->cmds[0]->fd_in != STDIN_FILENO)
	{
		if (dup2_and_close(mini->cmds[0]->fd_in, STDIN_FILENO))
		{
			reset_fds(mini);
			return (1);
		}
	}
	if (mini->cmds[0]->fd_out != STDOUT_FILENO)
	{
		if (dup2_and_close(mini->cmds[0]->fd_out, STDOUT_FILENO))
		{
			reset_fds(mini);
			return (1);
		}
	}
	if (built_in_exe(mini, mini->cmds[0]))
	{
		clean_cmds(mini->cmds);
		reset_fds(mini);
		return (1);
	}
	if (reset_fds(mini))
		return (1);
	return (0);
}

// Executes command
// Check why env_array parsed based on min->env is not working

int	execute_cmd(t_shell *mini, t_cmd *cmd)
{
	char	**env_array;

	env_array = env_to_array(mini->env);
	sig_reseted();
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
	{
		perror(cmd->command);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

// int	execute_single_cmd(t_shell *mini, t_cmd *cmd)
// {
// 	char	**env_array;

// 	env_array = env_to_array(mini->env);
// 	sig_reseted();
// 	if (is_this_builtin_cmd(cmd))
// 	{
// 		if (built_in_exe(mini, cmd))
// 			return (1);
// 	}
// 	else
// 	{
// 		if (execve(cmd->cmd_path, cmd->args, envp) == -1)
// 		{
// 			perror(cmd->command);
// 			// free everything
// 			return (1);
// 		}
// 	}
// 	return (0);
// }
