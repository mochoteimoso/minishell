/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:28:23 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/25 13:17:13 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		execute_pipeline(t_shell *mini);
int		handle_single_cmd(t_shell *mini);
int		fork_and_execute(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i);
int		fork_single_cmd(t_shell *mini, t_cmd *cmd);
int		execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd);
int		execute_forked_cmd(t_shell *mini, t_cmd *cmd);
void	close_fds_and_pipes(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i);
void	close_pipes(t_shell *mini, int pipe_fd[2]);
void	wait_children(t_shell *mini);

// Initializes an array of pipe_fds based on the number of pipes
// executes a single command if there are no pipes
// sets up pipeline and forks child processes

int	execute_pipeline(t_shell *mini) 
{
	int		pipe_fd[2];
	int		i;
	t_cmd	*cmd;
	
	i = 0;
	if (mini->cmd_count == 1 && is_this_built(mini->cmds[0]->command))
	{
		if (handle_single_cmd(mini))
			return (1);
		return (0);
	}
	mini->pids = ft_calloc(mini->cmd_count, sizeof(pid_t));
	if (!mini->pids)
	{
		clean_cmds(mini->cmds);   // create a function that also sets pointer to null!
		return (1);
	}
	if (mini->cmd_count == 1)
	{
		if (fork_single_cmd(mini, mini->cmds[0]))
			return (1);
	}
	while (i < mini->cmd_count)
	{
		cmd = mini->cmds[i];
		if (i < mini->cmd_count - 1 && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		} 
		if (fork_and_execute(mini, cmd, pipe_fd, i) == -1)
			return (1);
		close_fds_and_pipes(mini, cmd, pipe_fd, i);
		i++;
	}
	close(mini->prev_pipe);
	clean_cmds(mini->cmds);
	wait_children(mini);
	return (0);
}

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
		else if (WIFSIGNALED(status))
			printf("Child %d terminated by signal %d\n", mini->pids[i], WTERMSIG(status));
		i++;
	}
}

int	fork_single_cmd(t_shell *mini, t_cmd *cmd)
{
	sig_handler_changer();
	mini->pids[0] = fork();
	if (mini->pids[0] == -1)
	{
		perror("fork");
		return (1);
	}
	else if (mini->pids[0] == 0)
	{
		if (cmd->fd_in != STDIN_FILENO)
		{
			if (dup2_and_close(cmd->fd_in, STDIN_FILENO))
				return (1);
		}
		if (cmd->fd_out != STDOUT_FILENO)
		{
			if (dup2_and_close(cmd->fd_out, STDOUT_FILENO))
				return (1);
		}
	}
	execute_forked_cmd(mini, cmd);
	return (0);
}

void	close_fds_and_pipes(t_shell *mini, t_cmd *cmd, int pipe_fd[2], int i)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	if (i > 0)
	{
		close(mini->prev_pipe);
		//mini->prev_pipe = -1;
	}
	
	if (i < mini->cmd_count - 1)
	{
		mini->prev_pipe = pipe_fd[0];  // Store the current pipe's read end for the next command
		close(pipe_fd[1]);
	}
}

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
		printf("Command: %s\n", cmd->command);
		printf("cmd->fd_in: %d, cmd->fd_out: %d\n", cmd->fd_in, cmd->fd_out);
		printf("mini->prev_pipe: %d\n", mini->prev_pipe);
		printf("pipe_fd[0]: %d, pipe_fd[1]: %d\n", pipe_fd[0], pipe_fd[1]);
		if (dup_input(mini, cmd, i)) // redirect input 
			return (1);
		if (dup_output(cmd, pipe_fd, mini->cmd_count, i)) //redirect output
			return (1);
		if (is_this_built(cmd->command))
		{
			if (execute_forked_builtin_cmd(mini, cmd))
				return (1);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (execute_forked_cmd(mini, cmd))
				return (1);
		}
	}
	return (0);
}

// Executes single command if there are no pipes
// duplicates fd based on fd_in and fd_out

int	handle_single_cmd(t_shell *mini)
{
	if (save_fds(mini))
		return (1);
	if (mini->cmds[0]->fd_in != STDIN_FILENO)
	{
		if (dup2_and_close(mini->cmds[0]->fd_in, STDIN_FILENO))
		{
			reset_fds(mini);
			return (1);
		}
	}
	if (mini->cmds[0]->fd_out != STDOUT_FILENO)
	{
		if (dup2_and_close(mini->cmds[0]->fd_out, STDOUT_FILENO))
		{
			reset_fds(mini);
			return (1);
		}
	}
	if (built_in_exe(mini, mini->cmds[0]))
	{
		clean_cmds(mini->cmds);
		reset_fds(mini);
		return (1);
	}
	if (reset_fds(mini))
		return (1);
	mini->cmds[0]->fd_in = -1; // are these needed here??
	mini->cmds[0]->fd_out = -1;
	return (0);
}

// Executes command
// Check why env_array parsed based on min->env is not working

int	execute_forked_builtin_cmd(t_shell *mini, t_cmd *cmd)
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

int	execute_forked_cmd(t_shell *mini, t_cmd *cmd)
{
	char	**env_array;
	
	env_array = env_to_array(mini->env);
	sig_reseted();
	//printf("Command: %s\n", cmd->command);
	//printf("mini->prev_pipe: %d\n", mini->prev_pipe);
	//printf("STDIN: %d, STDOUT: %d\n", STDIN_FILENO, STDOUT_FILENO);
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
	{
		perror(cmd->command);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

/*
int	execute_cmd(t_shell *mini, t_cmd *cmd)
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
} */

/*
int	execute_single_cmd(t_shell *mini, t_cmd *cmd)
{
	char	**env_array;
	
	env_array = env_to_array(mini->env);
	if (is_this_builtin_cmd(cmd))
	{
		if (built_in_exe(mini, cmd))
			return (1);
	}
	else
	{
		if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
		{
			perror(cmd->command);
			return (1);
		}
	}
	return (0);
} */
