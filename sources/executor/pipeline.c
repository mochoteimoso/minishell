/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 17:36:36 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			execute_pipeline(t_shell *mini);
static int	handle_single_builtin_cmd(t_shell *mini);
static int	pipe_and_fork(t_shell *mini);
static int	allocate_pipes_array(t_shell *mini);
static int	allocate_pipes(t_shell *mini);

/**
 * execute_pipeline - Executes a pipeline of commands,
 * 					  including single built-in commands.
 *
 * @mini: Pointer to the shell structure containing command and pipeline data.
 *
 * Handles execution of either a single built-in command
 * or a pipeline of commands.
 * Allocates necessary resources (like pipes and process IDs),
 * manages redirections, forks processes for each command in the pipeline,
 * waits for children, and cleans up.
 *
 * Returns the exit status of the last command executed.
 */
int	execute_pipeline(t_shell *mini)
{
	if (mini->cmd_count == 1 && mini->cmds[0]->command
		&& is_this_built(mini->cmds[0]->command))
	{
		if (handle_single_builtin_cmd(mini))
			return (mini->exit_stat);
		clean_cmds(mini->cmds);
		mini->exit_stat = 0;
		return (mini->exit_stat);
	}
	mini->pids = ft_calloc(mini->cmd_count, sizeof(pid_t));
	if (!mini->pids)
	{
		clean_cmds(mini->cmds);
		return (mini->exit_stat = 1);
	}
	if (pipe_and_fork(mini))
	{
		cleaner_for_failure(mini);
		return (mini->exit_stat);
	}
	wait_children(mini);
	unlink_all_heredocs(mini);
	cleaner_for_success(mini);
	return (mini->exit_stat);
}

/**
 * handle_single_builtin_cmd - Executes a single built-in command
 * 							   without forking.
 *
 * @mini: Pointer to the shell structure containing the built-in command.
 *
 * Resolves file descriptors for input and output redirection,
 * executes the built-in command,
 * and restores the original file descriptors. Cleans up resources in case
 * of failure.
 *
 * Returns 0 on success or 1 on failure.
 */
static int	handle_single_builtin_cmd(t_shell *mini)
{
	if (resolve_fd(mini->cmds[0]) || save_fds(mini))
	{
		mini->exit_stat = mini->cmds[0]->cmd_exit;
		clean_cmds(mini->cmds);
		return (1);
	}
	if (redirect_fd(mini->cmds[0]->fd_in, STDIN_FILENO)
		|| redirect_fd(mini->cmds[0]->fd_out, STDOUT_FILENO))
		return (1);
	if (built_in_exe(mini, mini->cmds[0]))
	{
		clean_cmds(mini->cmds);
		reset_fds(mini);
		return (1);
	}
	if (mini->cmds[0]->fd_in != STDIN_FILENO
		|| (mini->cmds[0]->fd_out != STDOUT_FILENO))
	{
		if (reset_fds(mini))
			return (1);
	}
	return (0);
}

/**
 * pipe_and_fork - Handles the creation of pipes
 * 				   and forks processes for a pipeline.
 *
 * @mini: Pointer to the shell structure containing command and pipeline data.
 *
 * Allocates pipes for inter-process communication if required,
 * forks processes for each command in the pipeline,
 * and sets up redirections. Closes unnecessary file
 * descriptors. Returns 0 on success or 1 on failure.
 */
static int	pipe_and_fork(t_shell *mini)
{
	int		i;
	t_cmd	*cmd;

	i = 0;
	if (mini->cmd_count > 1)
	{
		if (allocate_pipes(mini))
			return (1);
	}
	while (i < mini->cmd_count)
	{
		cmd = mini->cmds[i];
		if (fork_and_execute(mini, cmd, i) == -1)
			return (1);
		sig_handler_changer();
		close_fds_and_pipes(mini, i);
		i++;
	}
	return (0);
}

/**
 * allocate_pipes - Allocates memory for pipes and initializes them.
 *
 * @mini: Pointer to the shell structure containing pipeline data.
 *
 * Allocates memory for the pipes used in the pipeline,
 * creates the pipe file descriptors, and ensures proper cleanup
 * in case of allocation failure.
 *
 * Returns 0 on success or 1 on failure.
 */
static int	allocate_pipes(t_shell *mini)
{
	mini->pipes = malloc(sizeof(int *) * (mini->cmd_count - 1));
	if (!mini->pipes)
	{
		perror("malloc");
		return (1);
	}
	if (allocate_pipes_array(mini))
		return (1);
	if (create_pipes(mini))
		return (1);
	return (0);
}

/**
 * allocate_pipes_array - Allocates memory for each pipe pair in the pipeline.
 *
 * @mini: Pointer to the shell structure containing pipeline data.
 *
 * Allocates memory for individual pipe file descriptor pairs. Ensures cleanup of
 * previously allocated resources in case of failure.
 *
 * Returns 0 on success or 1 on failure.
 */
static int	allocate_pipes_array(t_shell *mini)
{
	int	i;

	i = 0;
	while (i < mini->cmd_count - 1)
	{
		mini->pipes[i] = ft_calloc(2, sizeof(int));
		if (!mini->pipes[i])
		{
			perror("malloc");
			while (i > 0)
			{
				i--;
				free(mini->pipes[i]);
			}
			free(mini->pipes);
			return (1);
		}
		i++;
	}
	return (0);
}
