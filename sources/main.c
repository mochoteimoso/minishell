/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/07 18:06:59 by henbuska         ###   ########.fr       */
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
	//printf("Struct 2: segment: %s\n", sh->cmds[2]->segment);
	//printf("Struct 0: heredoc_delim: %s\n", sh->cmds[0]->heredoc_delim);
	//printf("Struct 0: heredoc: %d\n", sh->cmds[0]->heredoc);
	if (parse_input(sh))
		return (1);
	int i = 0;
	while (sh->cmds[i])
	{
		printf("Struct %d: segment: %s\n", i, sh->cmds[i]->segment);
		printf("Struct %d: redirect_in: %s\n", i, sh->cmds[i]->redirect_in);
		printf("Struct %d: redirect_out %s\n", i, sh->cmds[i]->redirect_out);
		printf("Struct %d: append %s\n", i, sh->cmds[i]->append);
		printf("Struct %d: heredoc: %d\n", i, sh->cmds[i]->heredoc);
		printf("Struct %d: heredoc_delim: %s\n", i, sh->cmds[i]->heredoc_delim);
		i++;
	}
	//if (parse_input(mini, *cmds))
	//	return (1);
	return (0);
}

// initialize mini struct

void	initialize_struct(t_shell *sh) //, t_command **cmds)
{
	ft_memset(sh, 0, sizeof(t_shell));
	//ft_memset(cmds, 0, sizeof(t_command));
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

/*
int	main(int argc, char **argv)
{
	(void)argc;
	t_shell		sh;
	t_command	*cmds;
	
	initialize_struct(&sh); // &cmds);
	if (parse_and_validate_input(argv[1], &sh, &cmds))
		exit(EXIT_FAILURE);
	free(cmds);
	return (0);
} */

/*static int	init_shell(t_shell *sh, char **envp)
{
	t_env	*ll;
	int 	i;

	ll = ft_calloc(1, sizeof(t_env *));
	if (!ll)
		return (1);
	list_env(ll, envp);
	sh->env = ll;
	sh->envp = (char **)malloc(sizeof(char *) * (ft_array_len(envp) + 1));
	if (!sh->envp)
		error("Malloc fail");
	i = 0;
	while (envp[i])
	{
		sh->envp[i] = ft_strdup(envp[i]);
		if (sh->envp[i])
		{
			while (i > 0)
				free(sh->envp[--i]);
			free(sh->envp);
			return (1);
		}
		i++;
	}
	sh->envp[i] = NULL;
	return (0);
} */