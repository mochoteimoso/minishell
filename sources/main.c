/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/05 17:35:15 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_single_command(char *line)
{
	t_command	single_command;
	
	add_single_command(&single_command, line);
	printf("Command: %s\n", single_command.command);
}

int	parse_and_validate_input(char *line, t_mini *mini) //t_command *cmds)
{
	if (validate_input_syntax(line))
		return (1);
	if (split_input_by_pipes(line, mini) == NULL)
		return (1);
	//parse_input_segments(mini, cmds);
	int i = 0;
	while (mini->split_input[i])
	{
		printf("%s\n", mini->split_input[i]);
		i++;
	}
	//if (parse_input(line))
	//	return (1);
	return (0);
}

void	initialize_structs(t_command *cmds, t_mini *mini)
{
	ft_memset(mini, 0, sizeof(t_mini));
	ft_memset(cmds, 0, sizeof(t_command));
}

int	main(int argc, char **argv)
{
	(void)argc;
	int			pipe_count;
	t_mini		mini;
	t_command	cmds;
	
	initialize_structs(&cmds, &mini);
	pipe_count = count_pipes(argv[1]) + 1;
	if (parse_and_validate_input(argv[1], &mini))
		exit(EXIT_FAILURE);
	if (pipe_count == 1)
		handle_single_command(argv[1]);
	//else
	//	handle_multiple_commands(argv[1]);
	return (0);
}


