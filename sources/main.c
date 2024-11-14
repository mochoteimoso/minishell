/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/14 17:31:42 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

static int	init_shell(t_shell *mini, char **envp)
{
	int	i;

	i = 0;
	mini->cmds = NULL;
	mini->env = list_env(envp);
	mini->pending = copy_env(envp);
	to_alphabetical(mini->pending);
	mini->pipe_count = 0;
	mini->exit_stat = 0;
	return (0);
}

static int	built_in_exe(char *input, t_shell *mini)
{
	char	**cmd = ft_split(input, ' ');
	if (ft_strcmp(cmd[0], "exit") == 0)
		return (built_exit(mini, cmd));
	else if (ft_strcmp(cmd[0], "cd") == 0)
		return (built_cd(mini, cmd));
	else if (ft_strcmp(cmd[0], "echo") == 0)
		return (built_echo(cmd));
	else if (ft_strcmp(cmd[0], "env") == 0)
	 	return (built_env(mini));
	else if (ft_strcmp(cmd[0], "pwd") == 0 && cmd[1] == NULL)
		return (built_pwd(mini));
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (built_unset(mini, cmd));
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (built_export(mini, cmd));
	ft_free_array(cmd);
	return (0);
}

static int user_prompt(char **envp)
{
	char	*input;
	t_shell	*mini;

	mini = malloc(sizeof(t_shell));
	if (!mini)
		error("Malloc failed\n");
	init_shell(mini, envp);
	init_sig();
	while (1)
	{

		input = readline("minishell> ");
		if (input == NULL)
			break ;
		if (input && *input)
			add_history(input);
		if (parse_and_validate_input(input, mini))
			error("ALL IS BROKE!!\n");
		if (execute_pipeline(mini, envp))
			error("ALL IS TERRIBLY BROKEN\n");
		built_in_exe(input, mini);
	}
	return (0);
}


int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc != 1)
	{
		printf("Minishell doesn't take arguments\n");
		return (0);
	}
	user_prompt(envp);
	return (0);
}
