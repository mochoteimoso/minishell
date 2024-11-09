/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_of_structs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:09:13 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/09 14:37:47 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			count_pipes(char *line);
int			prepare_command_structs(t_shell *sh, char *input);
t_command	**allocate_cmd_array(int command_count);
void		initialize_command_struct(t_command *cmd);

// Counts the number of pipes in input string

int	count_pipes(char *line)
{
	int	i;
	int	pipe_count;

	i = 0;
	pipe_count = 0;
	while (line[i])
	{
		if (line[i] == '|' && !is_in_quotes(line, i))
			pipe_count++;
		i++;
	}
	return (pipe_count);
}

// Sets up an array of structs and allocates memory for individual structs

int	prepare_command_structs(t_shell *sh, char *input)
{
	int	i;
	int	command_count;

	command_count = count_pipes(input) + 1;
	sh->cmds = allocate_cmd_array(command_count);
	if (!sh->cmds)
	{
		printf("Failed to allocate memory for command array\n");
		return (1);
	}
	i = 0;
	while (i < command_count)
	{
		sh->cmds[i] = malloc(sizeof(t_command));
		if (!sh->cmds[i])
		{
			printf("Failed to allocate memory for struct\n");
			while (i > 0)
			{
				i--;
				free(sh->cmds[i]);
			}
			free(sh->cmds);
			return (1);
		}
		initialize_command_struct(sh->cmds[i]);
		i++;
	}
	printf("Allocated %d command struct(s)\n", command_count);
	return (0);
}

//Allocates memory for an array of structs

t_command	**allocate_cmd_array(int command_count)
{
	t_command	**cmds;

	cmds = ft_calloc(command_count, sizeof(t_command));
	if (!cmds)
	{
		printf("Failed to allocate memory for array of structs\n");
		return (NULL);
	}
	return (cmds);
}

// Initializes individual structs in the array

void	initialize_command_struct(t_command *cmd)
{
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->args_count = 0;
	cmd->redirect_in = NULL;
	cmd->redirect_out = NULL;
	cmd->redirect_type = 0;
	cmd->append = NULL;
	cmd->heredoc = false;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_content = NULL;
	cmd->env_vars = NULL;
	cmd->env_var_count = 0;
	cmd->exit_status = 0;
}
