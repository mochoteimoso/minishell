/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:08:05 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/04 15:34:36 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			fork_and_execute(t_shell *mini, t_cmd *cmd, int i);
static void	execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd, int i);
static int	execute_forked_cmd(t_shell *mini, t_cmd *cmd, int i);
void		close_unused_fds(t_shell *mini, int i);

// Forks child processes based on number of commands
// duplicates input/output

int	fork_and_execute(t_shell *mini, t_cmd *cmd, int i)
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
		close_unused_fds(mini, i);
		if (resolve_fd(cmd))
			exit_handler(mini, i, cmd->cmd_exit);
		if (dup_input(mini, cmd, i))
			exit_handler(mini, i, cmd->cmd_exit);
		if (dup_output(mini, cmd, mini->cmd_count, i))
			exit_handler(mini, i, cmd->cmd_exit);
		if (is_this_built(cmd->command))
			execute_forked_builtin_cmd(mini, cmd, i);
		else
			execute_forked_cmd(mini, cmd, i);
	}
	return (0);
}

// Executes builtin command and closes fds in case of failure

static void	execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd, int i)
{
	if (built_in_exe(mini, cmd))
	{
		exit_handler(mini, i, EXIT_FAILURE);
	}
	exit_for_success(mini, i, EXIT_SUCCESS);
}

// Executes non-builtin command
// Parses env_array to use in execve

static int	execute_forked_cmd(t_shell *mini, t_cmd *cmd, int i)
{
	char	**env_array;

	env_array = env_to_array(mini->env);
	if (!env_array)
		exit_handler(mini, i, 1);
	sig_reseted();
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
	{
		perror(cmd->command);
		exit_handler(mini, i, -1);
	}
	exit(EXIT_SUCCESS);
}

void	close_unused_fds(t_shell *mini, int i)
{
	int	j;

	j =  i + 1;
	if (mini->cmd_count >= 2 && i < mini->cmd_count - 1)
		close(mini->pipes[i][0]);
	while (j < mini->cmd_count - 1)
	{
		if (mini->pipes[j])
		{
			close(mini->pipes[j][0]);
			close(mini->pipes[j][1]);
		}
		j++;
	}
}
