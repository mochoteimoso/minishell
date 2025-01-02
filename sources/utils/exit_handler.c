/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 11:43:47 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 11:42:35 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_for_failure(t_shell *mini, int i, int exit_status);
void	exit_for_success(t_shell *mini, int i, int exit_status);
void	exit_for_single_cmd(t_shell *mini, int exit_status);
void	close_all_pipes(t_shell *mini);
void	hd_free(t_expand *arg, char *expan);

/**
 * exit_for_failure - Cleans up resources
 * and exits the shell with the given status upon failure.
 *
 * @mini: Pointer to the shell structure containing resources to free.
 * @i: Index of the current command being executed.
 * @exit_status: Exit status code to terminate the shell with.
 */
void	exit_for_failure(t_shell *mini, int i, int exit_status)
{
	close_all_pipes(mini);
	if (mini->cmds[i]->fd_out > 2)
		close(mini->cmds[i]->fd_out);
	if (mini->cmds[i]->fd_in > 2)
		close(mini->cmds[i]->fd_in);
	clean_env(mini->env, mini->pending);
	clean_cmds(mini->cmds);
	free(mini->pids);
	mini->pids = NULL;
	if (mini->cmd_count > 1)
		ft_free_int_arr_with_size(mini->pipes, mini->cmd_count - 1);
	free(mini);
	mini = NULL;
	exit (exit_status);
}

/**
 * exit_for_success - Cleans up resources
 * and exits the shell with the given status upon success.
 *
 * @mini: Pointer to the shell structure containing resources to free.
 * @i: Index of the current command being executed.
 * @exit_status: Exit status code to terminate the shell with.
 */
void	exit_for_success(t_shell *mini, int i, int exit_status)
{
	if (mini->cmds[i]->fd_out != STDOUT_FILENO && mini->cmds[i]->fd_out != -1)
		close(mini->cmds[i]->fd_out);
	if (mini->cmds[i]->fd_in != STDIN_FILENO && mini->cmds[i]->fd_in != -1)
		close(mini->cmds[i]->fd_in);
	clean_env(mini->env, mini->pending);
	clean_cmds(mini->cmds);
	free(mini->pids);
	mini->pids = NULL;
	if (mini->cmd_count > 1)
		ft_free_int_arr_with_size(mini->pipes, mini->cmd_count - 1);
	free(mini);
	mini = NULL;
	exit (exit_status);
}

/**
 * exit_for_single_cmd - Cleans up resources
 * and exits the shell for single command execution.
 *
 * @mini: Pointer to the shell structure containing resources to free.
 * @exit_status: Exit status code to terminate the shell with.
 */
void	exit_for_single_cmd(t_shell *mini, int exit_status)
{
	clean_env(mini->env, mini->pending);
	clean_cmds(mini->cmds);
	free(mini->pids);
	mini->pids = NULL;
	exit (exit_status);
}

/**
 * hd_free - Frees resources allocated during heredoc expansion.
 *
 * @arg: Pointer to the expansion structure containing the value to free.
 * @expan: Pointer to the expanded string to free.
 */
void	hd_free(t_expand *arg, char *expan)
{
	free(arg->value);
	free(expan);
}

/**
 * close_all_pipes - Closes all pipes used in the shell.
 *
 * @mini: Pointer to the shell structure containing the pipes to close.
 */
void	close_all_pipes(t_shell *mini)
{
	int	j;

	j = 0;
	while (j < mini->cmd_count - 1)
	{
		close(mini->pipes[j][0]);
		close(mini->pipes[j][1]);
		j++;
	}
}
