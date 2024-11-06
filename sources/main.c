/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/06 18:02:50 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	init_shell(t_shell *sh, char **envp)
{
	sh->env = ft_calloc(1, sizeof(t_env *));
	if (!sh->env)
		return (1);
	sh->env = list_env(envp);
	sh->exit_stat = 0;
	return (0);
}

static int	built_in_exe(char *input, t_shell *sh)
{
	char	**cmd = ft_split(input, ' ');
	// printf("cmd[0]: %s\ncmd[1]: %s\n", cmd[0], cmd[1]);
	if (ft_strcmp(cmd[0], "exit") == 0)
		return (built_exit(sh, cmd));
	else if (ft_strcmp(cmd[0], "cd") == 0)
		return (built_cd(sh, cmd));
	else if (ft_strcmp(cmd[0], "echo") == 0)
		return (built_echo(cmd));
	else if (ft_strcmp(cmd[0], "env") == 0)
	 	return (built_env(sh));
	else if (ft_strcmp(cmd[0], "pwd") == 0 && cmd[1] == NULL)
		return (built_pwd(sh));
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (built_unset(&sh->env, cmd));
	// else if (ft_strcmp(cmd, "export") == 0)
	// 	return (built_export(sh, cmd));
	return (0);
}

static int user_prompt(char **envp)
{
	char	*input;
	t_shell	*sh;

	sh = malloc(sizeof(t_shell));
	init_shell(sh, envp);
	init_sig();
	while (1)
	{

		input = readline("minishell> ");
		if (input == NULL)
			break ;
		if (input && *input)
			add_history(input);
		built_in_exe(input, sh);
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
