/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/04 14:45:43 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	init_shell(t_shell *sh, char **envp)
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
}

static built_in_exe(char *input, t_shell *sh, char **envp)
{
	char	**cmd = ft_split(input, ' ');

	if (ft_strcmp(cmd, "exit") == 0)
		return (built_exit(sh, cmd));
	else if (ft_strcmp(cmd, "cd") == 0)
		return (built_cd(sh, cmd));
	else if (ft_strcmp(cmd, "echo") == 0)
		return (built_echo(cmd));
	else if (ft_strcmp(cmd, "env") == 0)
		return (built_env(sh, envp));
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (built_pwd());
	else if (ft_strcmp(cmd, "unset") == 0)
		return (built_unset(sh, cmd));
	else if (ft_strcmp(cmd, "export") == 0)
		return (built_export(sh, cmd));
}

static int user_prompt(char **envp)
{
	char	*input;
	t_shell	sh;

	init_shell(&sh, envp);
	init_sig();
	while (1)
	{

		input = readline("minishell> ");
		if (input == NULL)
			break ;
		if (input && *input)
			add_history(input);
		built_in_exe(input, &sh, *envp);
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
