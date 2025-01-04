/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 11:43:47 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/03 15:35:13 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_for_failure(t_shell *mini, int i, int exit_status);
void	exit_for_success(t_shell *mini, int i, int exit_status);
void	clean_cmd_unlink(t_shell *mini);
void	close_all_pipes(t_shell *mini);
void	hd_free(t_expand *arg, char *expan);

/**
 * exit_for_failure - Cleans up resources and exits the shell on failure.
 *
 * @mini: Pointer to the shell structure containing resources to free.
 * @i: Index of the current command being executed.
 * @exit_status: Exit status code to terminate the shell with.
 *
 * This function performs a comprehensive cleanup, including closing file
 * descriptors,  unlinking heredocs, freeing memory, and releasing resources
 * before terminating the shell with the specified exit status.
 */
void	exit_for_failure(t_shell *mini, int i, int exit_status)
{
	close_all_pipes(mini);
	unlink_all_heredocs(mini);
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
 * clean_cmd_unlink - Cleans up command resources and unlinks heredocs.
 *
 * @mini: Pointer to the shell structure containing commands and heredocs.
 *
 * Unlinks all heredoc files used during execution
 * and frees the allocated memory for command structures.
 */
void	clean_cmd_unlink(t_shell *mini)
{
	unlink_all_heredocs(mini);
	clean_cmds(mini->cmds);
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
