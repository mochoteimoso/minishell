/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/05 19:07:40 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_and_validate_input(char *line, t_mini *mini)
{
	if (validate_input_syntax(line))
		return (1);
	if (split_input_by_pipes(line, mini) == NULL)
		return (1);
	prepare_command_structs(line);
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

void	initialize_structs(t_mini *mini)
{
	ft_memset(mini, 0, sizeof(t_mini));
	//ft_memset(cmds, 0, sizeof(t_command));
}

int	main(int argc, char **argv)
{
	(void)argc;
	t_mini		mini;
	
	initialize_structs(&mini);
	if (parse_and_validate_input(argv[1], &mini))
		exit(EXIT_FAILURE);
	return (0);
}


