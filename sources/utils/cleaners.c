/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:18:31 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:59:43 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		mini_cleaner(t_shell *mini);
void		clean_cmds(t_cmd **cmds);
void		cleaner_for_success(t_shell *mini);
void		cleaner_for_failure(t_shell *mini);
static void	free_and_close_pipes(t_shell *mini);

/**
 * mini_cleaner - Frees all allocated resources within the t_shell structure.
 *
 * @mini: Pointer to the shell structure containing resources to clean.
 *
 * Frees the environment, pending variables, command structures, process IDs,
 * and pipes associated with the shell.
 * Ensures no memory leaks when the shell exits.
 */
void	mini_cleaner(t_shell *mini)
{
	if (mini->env)
		clean_env(mini->env, NULL);
	if (mini->pending)
		ft_free_array(mini->pending);
	if (mini->cmds)
		clean_cmds(mini->cmds);
	if (mini->pids)
		free(mini->pids);
	if (mini->cmd_count > 1)
		ft_free_int_arr_with_size(mini->pipes, mini->cmd_count - 1);
}

/**
 * clean_cmds - Frees the command array and all associated data.
 *
 * @cmds: Array of pointers to t_cmd structures representing shell commands.
 *
 * Frees each command's arguments, command paths, redirection structures,
 * and finally the command array itself.
 */
void	clean_cmds(t_cmd **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		free(cmds[i]->seg);
		if (cmds[i]->command != NULL)
			free(cmds[i]->command);
		if (cmds[i]->cmd_path && cmds[i]->cmd_path != cmds[i]->command)
			free(cmds[i]->cmd_path);
		ft_free_array(cmds[i]->args);
		if (cmds[i]->redir_head)
			clean_redir(cmds[i]->redir_head);
		free(cmds[i]);
		i++;
	}
	free(cmds);
	cmds = NULL;
}

/**
 * cleaner_for_success - Cleans up resources after
 * a successful pipeline execution.
 *
 * @mini: Pointer to the shell structure containing resources to clean.
 *
 * Frees command structures, process IDs, and closes any open pipes
 * related to a successful command execution.
 */
void	cleaner_for_success(t_shell *mini)
{
	int	i;

	i = mini->cmd_count - 2;
	clean_cmds(mini->cmds);
	if (mini->pids)
	{
		free(mini->pids);
		mini->pids = NULL;
	}
	if (mini->cmd_count > 1 && mini->pipes)
	{
		if (mini->pipes[i][0] != -1)
		{
			if (mini->pipes[i][0] > 0)
				close(mini->pipes[i][0]);
		}
	}
	if (mini->cmd_count > 1)
		ft_free_int_arr_with_size(mini->pipes, mini->cmd_count - 1);
}

/**
 * cleaner_for_failure - Cleans up resources after a failed pipeline execution.
 *
 * @mini: Pointer to the shell structure containing resources to clean.
 *
 * Frees command structures, process IDs, and closes and frees pipes
 * associated with the failed execution.
 */
void	cleaner_for_failure(t_shell *mini)
{
	clean_cmds(mini->cmds);
	if (mini->pids)
	{
		free(mini->pids);
		mini->pids = NULL;
	}
	if (mini->cmd_count > 1 && mini->pipes)
		free_and_close_pipes(mini);
}

/**
 * free_and_close_pipes - Frees and closes all pipe file descriptors
 * 						  in the shell.
 *
 * @mini: Pointer to the shell structure containing the pipes to clean.
 *
 * Closes the read and write ends of each pipe and frees the memory associated
 * with the pipe array. Sets the pipe array pointer to NULL.
 */
static void	free_and_close_pipes(t_shell *mini)
{
	int	i;

	i = 0;
	while (i < mini->cmd_count - 1)
	{
		if (mini->pipes[i][0] > 0)
			close(mini->pipes[i][0]);
		if (mini->pipes[i][1] > 1)
			close(mini->pipes[i][1]);
		free(mini->pipes[i]);
		i++;
	}
	free(mini->pipes);
	mini->pipes = NULL;
}
