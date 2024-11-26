/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/26 19:02:25 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			execute_pipeline(t_shell *mini);
static int	handle_single_builtin_cmd(t_shell *mini);
static int	pipe_and_fork(t_shell *mini, int pipe_fd[2]);

// Sets up pipeline and forks child processes when needed
// Allocates memory for an array of pids to store IDs of child processes

int	execute_pipeline(t_shell *mini)
{
	int	pipe_fd[2];

	if (mini->cmd_count == 1 && is_this_built(mini->cmds[0]->command))
	{
		if (handle_single_builtin_cmd(mini))
		{
			clean_cmds(mini->cmds);
			mini->exit_stat = 1;
			return (mini->exit_stat);
		}
		return (0);
	}
	mini->pids = ft_calloc(mini->cmd_count, sizeof(pid_t));
	if (!mini->pids)
	{
		clean_cmds(mini->cmds);
		mini->exit_stat = 1;
		return (mini->exit_stat);
	}
	if (pipe_and_fork(mini, pipe_fd))
	{
		cleaner_for_main(mini);
		return (1);
	}
	//if (mini->prev_pipe != -1)
	//	close(mini->prev_pipe);
	wait_children(mini);
	close(pipe_fd[0]);
	cleaner_for_main(mini);
	return (0);
}

// Executes single builtin command in parent process
// duplicates fd based on fd_in and fd_out and resets STDIN and STDOUT

static int	handle_single_builtin_cmd(t_shell *mini)
{
	if (save_fds(mini))
		return (1);
	if (resolve_fd(mini->cmds[0]))
		return (1);
	if (mini->cmds[0]->fd_in != STDIN_FILENO)
	{
		if (dup2_and_close_in_main(mini, mini->cmds[0]->fd_in, STDIN_FILENO))
			return (1);
	}
	if (mini->cmds[0]->fd_out != STDOUT_FILENO)
	{
		if (dup2_and_close_in_main(mini, mini->cmds[0]->fd_out, STDOUT_FILENO))
			return (1);
	}
	if (built_in_exe(mini, mini->cmds[0]))
	{
		clean_cmds(mini->cmds);
		reset_fds(mini);
		return (1);
	}
	if (reset_fds(mini))
		return (1);
	return (0);
}

// Creates pipes when needed and forks child processes
// After forking, closes cmd-specific fds that were passed to child

static int	pipe_and_fork(t_shell *mini, int pipe_fd[2])
{
	int		i;
	t_cmd	*cmd;

	i = 0;
	while (i < mini->cmd_count)
	{
		cmd = mini->cmds[i];
		if (mini->cmd_count > 1 && i < mini->cmd_count - 1)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				return (1);
			}
		}
		if (fork_and_execute(mini, cmd, pipe_fd, i) == -1)
			return (1);
		close_fds_and_pipes(mini, cmd, pipe_fd, i);
		i++;
	}
	return (0);
}
