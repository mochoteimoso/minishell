/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 09:04:45 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/11 12:47:56 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_cmd(t_cmd *cmd, t_shell *mini)
{
	pid_t	pid;
	int		status;

	pid 
}

int	execution(t_shell *mini)
{
	int i = 0;

	while (mini->cmds[i])
	{
		if (execute_cmd(mini->cmds[i], mini))
			return (1);
		i++;
	}
	return (0);
}
