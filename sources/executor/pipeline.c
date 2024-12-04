/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/04 15:35:34 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			execute_pipeline(t_shell *mini);
static int	handle_single_builtin_cmd(t_shell *mini);
static int	pipe_and_fork(t_shell *mini);
static int	allocate_pipes(t_shell *mini);
static int	create_pipes(t_shell *mini);


// Sets up pipeline and forks child processes when needed
// Allocates memory for an array of pids to store IDs of child processes

int	execute_pipeline(t_shell *mini)
{
	if (mini->cmd_count == 1 && is_this_built(mini->cmds[0]->command))
	{
		if (handle_single_builtin_cmd(mini))
		{
			clean_cmds(mini->cmds);
			mini->exit_stat = 1;
			return (mini->exit_stat);
		}
		clean_cmds(mini->cmds);
		mini->exit_stat = 0;
		return (mini->exit_stat);
	}
	mini->pids = ft_calloc(mini->cmd_count, sizeof(pid_t));
	if (!mini->pids)
	{
		clean_cmds(mini->cmds);
		mini->exit_stat = 1;
		return (mini->exit_stat);
	}
	if (pipe_and_fork(mini))
	{
		cleaner_for_main(mini);
		return (1);
	}
	wait_children(mini);
	//close(mini->pipes[i][0]);
	cleaner_for_main(mini);
	return (0);
}

// Executes single builtin command in parent process
// duplicates fd based on fd_in and fd_out and resets STDIN and STDOUT

static int	handle_single_builtin_cmd(t_shell *mini)
{
	if (resolve_fd(mini->cmds[0]))
		return (1);
	if (save_fds(mini))
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
	if (mini->cmds[0]->fd_in != STDIN_FILENO || (mini->cmds[0]->fd_out != STDOUT_FILENO))
	{
		if (reset_fds(mini))
			return (1);
	}
	return (0);
}

// Creates pipes when needed and forks child processes
// After forking, closes cmd-specific fds that were passed to child

static int	pipe_and_fork(t_shell *mini)
{
	int		i;
	t_cmd	*cmd;

	i = 0;
	if (mini->cmd_count > 1)
	{
		if (allocate_pipes(mini))
			return (1);
	}
	while (i < mini->cmd_count)
	{
		cmd = mini->cmds[i];
		if (fork_and_execute(mini, cmd, i) == -1)
			return (1);
		close_fds_and_pipes(mini, i);
		i++;
	}
	return (0);
}

static int	allocate_pipes(t_shell *mini)
{
	int	i;
	
	mini->pipes = malloc(sizeof(int*) * (mini->cmd_count - 1));
	if (!mini->pipes)
	{
		perror("malloc");
		return (1);
	}
	i = 0;
	while (i < mini->cmd_count - 1)
	{
		mini->pipes[i] = ft_calloc(2, sizeof(int));
		if (!mini->pipes[i])
		{
			perror("malloc");
			while (i > 0) 
			{
				i--;
				free(mini->pipes[i]);
			}
			free(mini->pipes);
			return (1);
		}
		i++;
	}
	if (create_pipes(mini))
		return (1);
	return (0);
}

static int	create_pipes(t_shell *mini)
{
	int	i;

	i = 0;
	while (i < mini->cmd_count - 1)
	{
		if (pipe(mini->pipes[i]) == -1) 
		{
			perror("pipe");
			while (i >= 0) 
			{
				free(mini->pipes[i]);
				i--;
			}
			free(mini->pipes);
			return (1);
		}
		i++;
	}
	return (0);
}

void	close_all_pipes(t_shell *mini, int *pipes)
{
	int	i;
	
	if (!pipes)
		return;

	i = 0;
	while (i < (mini->cmd_count - 1) * 2)
	{
		close(pipes[i]);
		i++;
	}
	free(pipes);
}
