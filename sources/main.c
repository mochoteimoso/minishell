/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/04 19:12:39 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_single_command(char *line)
{
	t_command	single_command;
	
	add_single_command(&single_command, line);
	printf("Command: %s\n", single_command.command);
}


int	main(int argc, char **argv)
{
	(void)argc;
	int	pipe_count;
	
	pipe_count = count_pipes(argv[1]) + 1;
	if (pipe_count == 1)
		handle_single_command(argv[1]);
	//else
	//	handle_multiple_commands(argv[1]);	
	return (0);
}

