/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:01:57 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:53:43 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		create_pipes(t_shell *mini);
int		dup2_and_close_in_main(t_shell *mini, int old_fd, int new_fd);
void	close_fds_and_pipes(t_shell *mini, int i);
void	wait_children(t_shell *mini);
void	unlink_all_heredocs(t_shell *mini);

/**
 * create_pipes - Creates pipes for inter-process communication.
 *
 * @mini: Pointer to the shell structure containing pipe information.
 *
 * Loops through the required number of pipes
 * and initializes them using `pipe()`.
 * Returns 0 on success, or 1 if `pipe()` fails (printing an error message).
 */
int	create_pipes(t_shell *mini)
{
	int	i;

	i = 0;
	while (i < mini->cmd_count - 1)
	{
		if (pipe(mini->pipes[i]) == -1)
		{
			perror("pipe");
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * dup2_and_close_in_main - Duplicates and closes file descriptors safely.
 *
 * @mini: Pointer to the shell structure for managing resources.
 * @old_fd: The source file descriptor to duplicate.
 * @new_fd: The target file descriptor to redirect to.
 *
 * Ensures valid `old_fd`, duplicates it onto `new_fd` using `dup2()`,
 * and closes the original `old_fd`. If an error occurs, resets file descriptors
 * in the shell and returns 1; otherwise, returns 0.
 */
int	dup2_and_close_in_main(t_shell *mini, int old_fd, int new_fd)
{
	if (old_fd < 0)
	{
		ft_putendl_fd("Invalid file descriptor", 2);
		reset_fds(mini);
		return (1);
	}
	if (dup2(old_fd, new_fd) == -1)
	{
		close(old_fd);
		reset_fds(mini);
		return (1);
	}
	close(old_fd);
	return (0);
}

/**
 * close_fds_and_pipes - Closes file descriptors and pipes for a command.
 *
 * @mini: Pointer to the shell structure containing pipe information.
 * @i: The index of the current command in the pipeline.
 *
 * Closes write ends of the pipe for the first command, read and write ends
 * for middle commands, and read ends for the last command.
 */
void	close_fds_and_pipes(t_shell *mini, int i)
{
	if (mini->cmd_count > 1)
	{
		if (i == 0)
		{
			close(mini->pipes[i][1]);
		}
		else if (i < mini->cmd_count - 1)
		{
			close(mini->pipes[i - 1][0]);
			close(mini->pipes[i][1]);
		}
		else if (i == mini->cmd_count - 1)
		{
			close(mini->pipes[i - 1][0]);
		}
	}
}

/**
 * wait_children - Waits for all child processes to finish.
 *
 * @mini: Pointer to the shell structure containing process IDs.
 *
 * Uses `waitpid()` to wait for each child process and updates the shell's
 * exit status to reflect the last child's exit code.
 */
void	wait_children(t_shell *mini)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < mini->cmd_count)
	{
		waitpid(mini->pids[i], &status, 0);
		if (WIFEXITED(status))
			mini->exit_stat = WEXITSTATUS(status);
		i++;
	}
}

/**
 * unlink_all_heredocs - Removes all heredoc temporary files.
 *
 * @mini: Pointer to the shell structure containing command redirection data.
 *
 * Iterates through all commands and unlinks (deletes) temporary files
 * associated with heredoc redirections. Frees memory for the filenames.
 */
void	unlink_all_heredocs(t_shell *mini)
{
	t_redir	*current;
	int		i;

	i = 0;
	while (i < mini->cmd_count)
	{
		current = mini->cmds[i]->redir_head;
		while (current)
		{
			if (current->type == HEREDOC)
			{
				if (current->heredoc_name)
				{
					unlink(current->heredoc_name);
					free(current->heredoc_name);
					current->heredoc_name = NULL;
				}
			}
			current = current->next;
		}
		i++;
	}
}
