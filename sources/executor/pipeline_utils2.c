/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:49:57 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 15:50:38 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redirect_fd(int src_fd, int target_fd);

int	redirect_fd(int src_fd, int target_fd)
{
	if (src_fd != target_fd)
	{
		if (dup2(src_fd, target_fd) == -1)
		{
			perror("dup2 failed");
			close(src_fd);
			return (1);
		}
		close(src_fd);
	}
	return (0);
}
