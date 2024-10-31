/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/31 17:31:38 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	init_shell(t_shell *sh, char **envp)
{
	t_env	*ll;

	ll = ft_calloc(1, sizeof(t_env *));
	if (!ll)
		return (1);
	list_env(ll, envp);
	sh->env = ll;
	return (0);
}

static int user_prompt(char **envp)
{
	char	*input;
	t_shell	sh;

	init_shell(&sh, envp);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
			break ;
		if (input && *input)
			add_history(input);
	}
	return (0);
}


int	main(int argc, char **argv, char **envp)
{
	//int	exit_c;

	(void)argv;
	//exit_c = 0;
	if (argc != 1)
	{
		printf("Minishell doesn't take arguments\n");
		return (0);
	}
	user_prompt(envp);
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	int	exit_c;

// 	(void)argv;
// 	exit_c = 0;
// 	if (argc != 1)
// 	{
// 		printf("Minishell doesn't take arguments\n");
// 		return (0);
// 	}
// 	return (user_prompt(exit_c, envp));
// }
