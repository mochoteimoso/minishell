/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:08:05 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/26 19:15:59 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			fork_and_execute(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i);
static void	execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd);
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
		//close_unused_fds(mini, cmd, i);
		if (resolve_fd(cmd))
			exit_handler(mini, cmd->cmd_exit);
		printf("Cmds[%d]: fd_in = %d, fd_out = %d\n", i, cmd->fd_in, cmd->fd_out);
		printf("Cmds[%d]: pipe_fd[0] = %d, pipe_fd[1] = %d\n", i, pipe_fd[0], pipe_fd[1]);
		if (dup_input(mini, cmd, i))
			exit_handler(mini, cmd->cmd_exit);
		if (dup_output(cmd, pipe_fd, mini->cmd_count, i))
			exit_handler(mini, cmd->cmd_exit);
		if (is_this_built(cmd->command))
			execute_forked_builtin_cmd(mini, cmd);
		else
			execute_forked_cmd(mini, cmd);
	}
	return (0);
}

// Executes builtin command and closes fds in case of failure

static void	execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd)
{
	if (built_in_exe(mini, cmd))
	{
		close(cmd->fd_in);
		close(cmd->fd_out);
		exit_handler(mini, EXIT_FAILURE);
	}
	exit_handler(mini, EXIT_SUCCESS);
}

// Executes non-builtin command
// Parses env_array to use in execve

static int	execute_forked_cmd(t_shell *mini, t_cmd *cmd)
{
	char	**env_array;

	env_array = env_to_array(mini->env);
	if (!env_array)
		exit_handler(mini, 1);
	sig_reseted();
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
	{
		perror(cmd->command);
		exit_handler(mini, -1);
	}
	exit(EXIT_SUCCESS);
}
