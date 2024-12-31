/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:18:10 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 16:34:11 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	save_fds(t_shell *mini);
int	reset_fds(t_shell *mini);

/**
 * save_fds - Saves the current standard input/output file descriptors.
 *
 * @mini: Pointer to the shell structure for managing state.
 *
 * If the input or output file descriptors are redirected, duplicates the
 * current STDIN_FILENO and STDOUT_FILENO file descriptors to save their
 * states. Updates `mini->stdin_saved` and `mini->stdout_saved` respectively.
 * Returns 0 on success or 1 on failure with an updated exit status.
 */
int	save_fds(t_shell *mini)
{
	if (mini->cmds[0]->fd_in != STDIN_FILENO)
	{
		mini->stdin_saved = dup(STDIN_FILENO);
		if (mini->stdin_saved == -1)
		{
			mini->exit_stat = 1;
			return (perror("Failed to save STDIN"), 1);
		}
	}
	if (mini->cmds[0]->fd_out != STDOUT_FILENO)
	{
		mini->stdout_saved = dup(STDOUT_FILENO);
		if (mini->stdout_saved == -1)
		{
			if (mini->stdin_saved != -1)
				close(mini->stdin_saved);
			mini->exit_stat = 1;
			return (perror("Failed to save STDOUT"), 1);
		}
	}
	return (0);
}

/**
 * reset_fds - Restores the saved standard input/output file descriptors.
 *
 * @mini: Pointer to the shell structure for managing state.
 *
 * If `mini->stdin_saved` or `mini->stdout_saved` were saved earlier, restores
 * the original STDIN_FILENO or STDOUT_FILENO using `dup2_and_close`.
 * Clears saved descriptors after restoration. Returns 0 on success or 1 on
 * failure with an updated exit status.
 */
int	reset_fds(t_shell *mini)
{
	if (mini->stdin_saved != -1 && mini->stdin_saved != STDIN_FILENO)
	{
		if (dup2_and_close(mini->stdin_saved, STDIN_FILENO))
		{
			perror("Failed to restore original STDIN");
			mini->exit_stat = 1;
			mini->stdin_saved = -1;
			return (1);
		}
		mini->stdin_saved = -1;
	}
	if (mini->stdout_saved != -1 && mini->stdin_saved != STDOUT_FILENO)
	{
		if (dup2_and_close(mini->stdout_saved, STDOUT_FILENO))
		{
			perror("Failed to restore original STDOUT");
			mini->exit_stat = 1;
			mini->stdout_saved = -1;
			return (1);
		}
		mini->stdout_saved = -1;
	}
	return (0);
}
