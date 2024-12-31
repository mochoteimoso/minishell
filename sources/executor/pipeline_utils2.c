/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:49:57 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 16:30:56 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redirect_fd(int src_fd, int target_fd);

/**
 * redirect_fd - Redirects one file descriptor to another.
 *
 * @src_fd: The source file descriptor to be redirected.
 * @target_fd: The target file descriptor to redirect to.
 *
 * Uses `dup2` to duplicate `src_fd` onto `target_fd`. If the descriptors differ,
 * the function handles the duplication and closes the original `src_fd`.
 * Returns 0 on success, or 1 if `dup2` fails (printing an error message).
 */
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
