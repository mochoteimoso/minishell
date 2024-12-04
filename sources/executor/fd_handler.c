/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:18:10 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/04 11:05:39 by henbuska         ###   ########.fr       */
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
