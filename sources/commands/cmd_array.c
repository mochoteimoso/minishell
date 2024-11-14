/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:09:13 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/14 18:47:02 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			count_pipes(char *line);
int			prepare_command_structs(t_shell *mini, char *input);
t_cmd		**allocate_cmd_array(int command_count);
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


int	prepare_command_structs(t_shell *mini, char *input)
{
	int	i;
	int	command_count;

	command_count = count_pipes(input) + 1;
	mini->pipe_count = command_count - 1;
	mini->cmds = allocate_cmd_array(command_count);
	if (!mini->cmds)
	{
		printf("Failed to allocate memory for command array\n");
		return (1);
	}
	i = 0;
	while (i < command_count)
	{
		mini->cmds[i] = malloc(sizeof(t_cmd));
		if (!mini->cmds[i])
		{
			printf("Failed to allocate memory for struct\n");
			while (i > 0)
			{
				i--;
				free(mini->cmds[i]);
			}
			free(mini->cmds);
			return (1);
		}
		initialize_command_struct(mini->cmds[i]);

		i++;
	}
	printf("Allocated %d command struct(s)\n", command_count);
	return (0);
}

//Allocates memory for an array of structs

t_cmd	**allocate_cmd_array(int command_count)
{
	t_cmd	**cmds;

	cmds = ft_calloc(command_count, sizeof(t_cmd));
	if (!cmds)
	{
		printf("Failed to allocate memory for array of structs\n");
		return (NULL);
	}
	return (cmds);
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
	cmd->exit_status = 0;
}
