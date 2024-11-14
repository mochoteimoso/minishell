/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/14 17:55:50 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

static void printer(t_shell *mini)
{
	int i = 0;

	while (mini->cmds[i])
	{
		printf("\n");
		printf("|*************************************************|\n");
		printf("Struct %d:\n", i);
		printf("segment: %s\n", mini->cmds[i]->segment);
		printf("command: %s\n", mini->cmds[i]->command);
		if (mini->cmds[i]->args)
		{
			int j = 0;
			while (mini->cmds[i]->args[j])
			{
				printf("arg %d: {%s}\n", j, mini->cmds[i]->args[j]);
				j++;
			}
		}
		t_redir *redir = mini->cmds[i]->redir_head;
		int redir_index = 0;
		while (redir)
		{
			printf("Redir\n");
			printf("Redirection %d - type: %d\n", redir_index, redir->type);
			printf("Redirection %d - file: %s\n", redir_index, redir->file);
			printf("Redirection %d - delimiter: %s\n", redir_index, redir->delimiter);
			redir = redir->next;
			redir_index++;
		}
		printf("|*************************************************|\n");
		printf("\n");
		i++;
	}
}

static int	init_shell(t_shell *mini, char **envp)
{
	int	i;

	i = 0;
	mini->cmds = NULL;
	mini->env = list_env(envp);
	mini->pending = copy_env(envp);
	to_alphabetical(mini->pending);
	mini->exit_stat = 0;
	return (0);
}

static int	built_in_exe(t_shell *mini)
{
	int i;

	i = 0;
	while (mini->cmds[i])
	{
		if (ft_strcmp(mini->cmds[i]->command, "exit") == 0)
			built_exit(mini, mini->cmds[i]);
		else if (ft_strcmp(mini->cmds[i]->command, "cd") == 0)
			return (built_cd(mini, mini->cmds[i]));
		else if (ft_strcmp(mini->cmds[i]->command, "echo") == 0)
			return (built_echo(mini->cmds[i]));
		else if (ft_strcmp(mini->cmds[i]->command, "env") == 0)
		 	return (built_env(mini));
		else if (ft_strcmp(mini->cmds[i]->command, "pwd") == 0 && !mini->cmds[i]->args[1])
			return (built_pwd(mini));
		else if (ft_strcmp(mini->cmds[i]->command, "unset") == 0)
			return (built_unset(mini, mini->cmds[i]));
		else if (ft_strcmp(mini->cmds[i]->command, "export") == 0)
			return (built_export(mini, mini->cmds[i]));
		i++;
	}
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
		printer(mini);
		built_in_exe(mini);
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
