/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/09 14:23:26 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_and_validate_input(char *input, t_shell *sh)
{
	if (validate_input_syntax(input))
		return (1);
	if (prepare_command_structs(sh, input))
		return (1);
	if (split_input_by_pipes(input, sh))
		return (1);
	if (parse_input(sh))
		return (1);
	int i = 0;
	while (sh->cmds[i])
	{
		printf("\n");
		printf("Struct %d: segment: %s\n", i, sh->cmds[i]->segment);
		printf("Struct %d: command: %s\n", i, sh->cmds[i]->command);
		printf("Struct %d: arg 1: %s\n", i, sh->cmds[i]->args[0]);
		printf("Struct %d: arg 2: %s\n", i, sh->cmds[i]->args[1]);
		printf("Struct %d: redirect_in: %s\n", i, sh->cmds[i]->redirect_in);
		printf("Struct %d: redirect_out %s\n", i, sh->cmds[i]->redirect_out);
		printf("Struct %d: append %s\n", i, sh->cmds[i]->append);
		printf("Struct %d: heredoc: %d\n", i, sh->cmds[i]->heredoc);
		printf("Struct %d: heredoc_delim: %s\n", i, sh->cmds[i]->heredoc_delim);
		printf("\n");
		i++;
	}
	return (0);
}

// initialize mini struct

void	initialize_struct(t_shell *sh)
{
	ft_memset(sh, 0, sizeof(t_shell));
}

static	int user_prompt()
{
	char		*input;
	t_shell		sh;

	//init_shell(&sh, envp);
	//init_sig();
	while (1)
	{
		initialize_struct(&sh);
		input = readline("minishell> ");
		if (input == NULL)
			break ;
		if (input && *input)
			add_history(input);	
		if (parse_and_validate_input(input, &sh))
			return (1);
	}
	return (0);
}

int	main(int argc, char **argv) //, char **envp)
{
	//int	exit_c;
	(void)argv;
	//exit_c = 0;
	if (argc != 1)
	{
		printf("Minishell doesn't take arguments\n");
		return (0);
	}
	user_prompt();
	return (0);
}

