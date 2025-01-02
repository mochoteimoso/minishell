/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:08:05 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:53:16 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			fork_and_execute(t_shell *mini, t_cmd *cmd, int i);
static int	setup_fds_and_check(t_shell *mini, t_cmd *cmd, int i);
static void	execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd, int i);
static int	execute_forked_cmd(t_shell *mini, t_cmd *cmd, int i);
void		close_unused_fds(t_shell *mini, int i);

/**
 * fork_and_execute - Forks a process and executes a command.
 *
 * @mini: Pointer to the shell structure containing environment details.
 * @cmd: Pointer to the command structure containing command details.
 * @i: Index of the command in the pipeline.
 *
 * Creates a new process using `fork`. The child process handles:
 *  - Closing unused file descriptors.
 *  - Setting up input and output redirections.
 *  - Executing either a built-in command or an external command.
 * The parent process continues managing other commands in the pipeline.
 * Returns 0 on success or -1 if `fork` fails.
 */
int	fork_and_execute(t_shell *mini, t_cmd *cmd, int i)
{
	sig_reseted();
	mini->pids[i] = fork();
	if (mini->pids[i] == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (mini->pids[i] == 0)
	{
		close_unused_fds(mini, i);
		if (setup_fds_and_check(mini, cmd, i))
			exit_for_failure(mini, i, cmd->cmd_exit);
		if (!cmd->command)
			exit_for_failure(mini, i, 0);
		if (is_this_built(cmd->command))
			execute_forked_builtin_cmd(mini, cmd, i);
		else
		{
			if (get_cmd_path(mini, cmd))
				exit_for_failure(mini, i, cmd->cmd_exit);
			execute_forked_cmd(mini, cmd, i);
		}
	}
	return (0);
}

/**
 * setup_fds_and_check - Resolves and duplicates file descriptors
 * 						 for the command.
 *
 * @mini: Pointer to the shell structure containing environment details.
 * @cmd: Pointer to the command structure containing command details.
 * @i: Index of the command in the pipeline.
 *
 * Sets up input and output file descriptors for the command by:
 *  - Resolving redirections.
 *  - Duplicating input and output file descriptors.
 * Returns 0 on success or 1 on failure.
 */
static int	setup_fds_and_check(t_shell *mini, t_cmd *cmd, int i)
{
	if (resolve_fd(cmd))
		return (1);
	if (dup_input(mini, cmd, i))
		return (1);
	if (dup_output(mini, cmd, mini->cmd_count, i))
		return (1);
	return (0);
}

/**
 * execute_forked_builtin_cmd - Executes a built-in command in the child process.
 *
 * @mini: Pointer to the shell structure containing environment details.
 * @cmd: Pointer to the command structure containing command details.
 * @i: Index of the command in the pipeline.
 *
 * Executes a built-in command within the child process. On failure,
 * it exits using `exit_for_failure`. On success, it exits using
 * `exit_for_success`.
 */
static void	execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd, int i)
{
	if (built_in_exe(mini, cmd))
	{
		exit_for_failure(mini, i, EXIT_FAILURE);
	}
	exit_for_success(mini, i, EXIT_SUCCESS);
}

/**
 * execute_forked_cmd - Executes an external command in the child process.
 *
 * @mini: Pointer to the shell structure containing environment details.
 * @cmd: Pointer to the command structure containing command details.
 * @i: Index of the command in the pipeline.
 *
 * Converts the environment list to an array and calls `execve` to execute
 * the external command. If `execve` fails, it cleans up resources and exits
 * using `exit_for_failure`.
 */
static int	execute_forked_cmd(t_shell *mini, t_cmd *cmd, int i)
{
	char	**env_array;

	env_array = env_to_array(mini->env);
	if (!env_array)
		exit_for_failure(mini, i, 1);
	sig_reseted();
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
	{
		ft_free_array(env_array);
		perror(cmd->command);
		exit_for_failure(mini, i, -1);
	}
	exit(EXIT_SUCCESS);
}

/**
 * close_unused_fds - Closes unused file descriptors in the child process.
 *
 * @mini: Pointer to the shell structure containing environment details.
 * @i: Index of the command in the pipeline.
 *
 * Closes unused file descriptors to avoid resource leaks in the child process.
 * Ensures proper management of pipe file descriptors for commands in a pipeline.
 */
void	close_unused_fds(t_shell *mini, int i)
{
	int	j;

	j = i + 1;
	if (mini->cmd_count >= 2 && i < mini->cmd_count - 1)
		close(mini->pipes[i][0]);
	while (j < mini->cmd_count - 1)
	{
		if (mini->pipes[j])
		{
			close(mini->pipes[j][0]);
			close(mini->pipes[j][1]);
		}
		j++;
	}
}
