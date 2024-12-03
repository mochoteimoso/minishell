/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 11:43:47 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/26 12:51:14 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_handler(t_shell *mini, int exit_status)
{
	clean_cmds(mini->cmds);
	mini->exit_stat = exit_status;
	exit (exit_status);
// free pids?
}