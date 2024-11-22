/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:18:10 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/20 18:18:55 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	save_fds(t_shell *mini)
{
	mini->stdin_saved = dup(STDIN_FILENO);
	if (mini->stdin_saved == -1)
		return (perror("Failed to save STDIN"), 1);
	mini->stdout_saved = dup(STDOUT_FILENO);
	if (mini->stdout_saved == -1)
	{
		close(mini->stdin_saved);
		return (perror("Failed to save STDOUT"), 1);
	}
	return (0);
}

int	reset_fds(t_shell *mini)
{
	if (dup2(mini->stdin_saved, STDIN_FILENO) == -1 ||
		dup2(mini->stdout_saved, STDOUT_FILENO) == -1)
	{
		perror("Failed to restore original FDs");
		close(mini->stdin_saved);
		close(mini->stdout_saved);
		return (1);
	}
	close(mini->stdin_saved);
	close(mini->stdout_saved);
	return (0);
}
