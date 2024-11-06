/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/06 15:35:47 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_and_validate_input(char *line, t_mini *mini, t_command **cmds)
{
	if (validate_input_syntax(line))
		return (1);
	if (split_input_by_pipes(line, mini))
		return (1);
	printf("mini->split_input[0]: %s\n", mini->split_input[0]);
	if (prepare_command_structs(line, cmds))
		return (1);
	printf("mini->split_input[0]: %s\n", mini->split_input[0]);
	printf("Struct element 0: heredoc_delim: %s\n", cmds[0]->heredoc_delim);
	printf("Struct element 0: heredoc: %d\n", cmds[0]->heredoc);
	parse_input_segments(mini, *cmds);
	printf("Struct element 0: heredoc_delim: %s\n", cmds[0]->heredoc_content);
	printf("Struct element 1: heredoc: %d\n", cmds[0]->heredoc);
	//if (parse_input(mini, *cmds))
	//	return (1);
	return (0);
}

// initialize mini struct

void	initialize_struct(t_mini *mini) //, t_command **cmds)
{
	ft_memset(mini, 0, sizeof(t_mini));
	//ft_memset(cmds, 0, sizeof(t_command));
}

int	main(int argc, char **argv)
{
	(void)argc;
	t_mini		mini;
	t_command	*cmds;
	
	initialize_struct(&mini); // &cmds);
	if (parse_and_validate_input(argv[1], &mini, &cmds))
		exit(EXIT_FAILURE);
	free(cmds);
	return (0);
}


