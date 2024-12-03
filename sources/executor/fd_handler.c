/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:18:10 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/03 09:48:11 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Saves original STDIN and STDOUT before possible duplication

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

// Restores original STDIN and STDOUT after possible duplication

int	reset_fds(t_shell *mini)
{
	if (mini->stdin_saved != -1)
	{
		if (dup2(mini->stdin_saved, STDIN_FILENO) == -1)
		{
			perror("Failed to restore original STDIN");
			close(mini->stdin_saved);
			mini->exit_stat = 1;
			return (1);
		}
		close(mini->stdin_saved);
	}
	if (mini->stdout_saved != -1)
	{
		if (dup2(mini->stdout_saved, STDOUT_FILENO) == -1)
		{
			perror("Failed to restore original STDOUT");
			close(mini->stdout_saved);
			mini->exit_stat = 1;
			return (1);
		}
		close(mini->stdout_saved);
	}
	return (0);
}
