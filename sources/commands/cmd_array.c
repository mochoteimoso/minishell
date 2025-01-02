/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:09:13 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 13:03:02 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			count_pipes(char *line);
int			prepare_command_structs(t_shell *mini, char *input);
void		initialize_command_struct(t_cmd *cmd);

/**
 * count_pipes - Counts the number of unquoted pipe characters in a line.
 *
 * @line: The input line to process.
 *
 * Iterates through the input line, counting the number of `|` characters
 * that are not enclosed in quotes. This is used to determine the number
 * of commands in a pipeline.
 *
 * Returns: The number of unquoted pipe characters found in the line.
 */
int	count_pipes(char *line)
{
	int	i;
	int	pipe_count;

	i = 0;
	pipe_count = 0;
	while (line[i])
	{
		if (line[i] == '|' && !check_quotes(line, i))
			pipe_count++;
		i++;
	}
	return (pipe_count);
}

/**
 * cmd_struct_while - Allocates and initializes command structures.
 *
 * @mini: Pointer to the shell structure containing the command array.
 * @cmd_count: The total number of commands to allocate.
 *
 * Iterates through the `mini->cmds` array, allocating memory for each
 * command structure and initializing it using `initialize_command_struct`.
 * If allocation fails, frees previously allocated memory and returns 1.
 *
 * Returns: 0 on success, or 1 on allocation failure.
 */
static	int	cmd_struct_while(t_shell *mini, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		mini->cmds[i] = malloc(sizeof(t_cmd));
		if (!mini->cmds[i])
		{
			ft_putendl_fd("Failed to allocate memory for struct", 2);
			clean_cmds(mini->cmds);
			return (1);
		}
		initialize_command_struct(mini->cmds[i]);
		i++;
	}
	mini->cmds[cmd_count] = NULL;
	return (0);
}

/**
 * prepare_command_structs - Prepares the array of command structures.
 *
 * @mini: Pointer to the shell structure containing the command details.
 * @input: The input line to process.
 *
 * Calculates the number of commands based on the pipe count in the input
 * line and allocates memory for the command array. Calls `cmd_struct_while`
 * to allocate and initialize individual command structures.
 *
 * Returns: 0 on success, or 1 if memory allocation fails.
 */
int	prepare_command_structs(t_shell *mini, char *input)
{
	int	command_count;

	command_count = count_pipes(input) + 1;
	mini->cmd_count = command_count;
	mini->cmds = ft_calloc(command_count, sizeof(t_cmd));
	if (!mini->cmds)
	{
		ft_putendl_fd("Failed to allocate memory for command array", 2);
		return (1);
	}
	if (cmd_struct_while(mini, command_count))
		return (1);
	return (0);
}

/**
 * initialize_command_struct - Initializes the fields of a command structure.
 *
 * @cmd: Pointer to the command structure to initialize.
 *
 * Sets all fields of the command structure to their initial values:
 * `NULL` for pointers, `0` for integers, and `-1` for file descriptors.
 * This ensures a clean state before populating the structure.
 */
void	initialize_command_struct(t_cmd *cmd)
{
	cmd->command = NULL;
	cmd->cmd_path = NULL;
	cmd->cmd_index = 0;
	cmd->args = NULL;
	cmd->a_num = 0;
	cmd->redir_head = NULL;
	cmd->redir_tail = NULL;
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	cmd->cmd_exit = 0;
}
