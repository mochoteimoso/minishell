/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:09:13 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/11 18:27:10 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			count_pipes(char *line);
int			prepare_command_structs(t_shell *mini, char *input);
void		initialize_command_struct(t_cmd *cmd);

// Counts the number of pipes in input string

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

// Sets up an array of structs and allocates memory for individual structs

static	int	cmd_struct_while(t_shell *mini, int cmd_count)
{
	int i;
	
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

// Allocates memory for an array of structs

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

// Initializes individual structs in the array

void	initialize_command_struct(t_cmd *cmd)
{
	cmd->command = NULL;
	cmd->cmd_path = NULL;
	cmd->args = NULL;
	cmd->args_count = 0;
	cmd->redir_head = NULL;
	cmd->redir_tail = NULL;
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	cmd->cmd_exit = 0;
}
