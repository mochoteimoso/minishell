/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:08:05 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/26 10:03:29 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			fork_and_execute(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i);
static int	execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd);
static int	execute_forked_cmd(t_shell *mini, t_cmd *cmd);

// Forks child processes based on number of commands
// duplicates input/output

int	fork_and_execute(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i)
{
	sig_handler_changer();
	mini->pids[i] = fork();
	if (mini->pids[i] == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (mini->pids[i] == 0)
	{
		close_unused_fds(mini, cmd, i);
		if (dup_input(mini, cmd, i))
			return (1);
		if (dup_output(cmd, pipe_fd, mini->cmd_count, i))
			return (1);
		if (is_this_built(cmd->command))
		{
			if (execute_forked_builtin_cmd(mini, cmd))
				return (1);
			exit(EXIT_SUCCESS);
		}
		if (execute_forked_cmd(mini, cmd))
			return (1);
	}
	return (0);
}

// Executes builtin command and closes fds in case of failure

static int	execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd)
{
	if (built_in_exe(mini, cmd))
	{
		clean_cmds(mini->cmds);
		close(cmd->fd_in);
		close(cmd->fd_out);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

// Executes non-builtin command
// Parses env_array to use in execve

static int	execute_forked_cmd(t_shell *mini, t_cmd *cmd)
{
	char	**env_array;

	env_array = env_to_array(mini->env);
	sig_reseted();
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
	{
		perror(cmd->command);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
