/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/29 17:25:33 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int user_prompt(int exit_c, char **envp)
{
	char	*input;
	(void)envp;
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
			break ;
		if (input && *input)
			add_history(input);
	}
	return (exit_c);
}

int	main(int argc, char **argv, char **envp)
{
	int	exit_c;

	exit_c = 0;
	(void)argc;
	(void)argv;
	return (user_prompt(exit_c, envp));
}
