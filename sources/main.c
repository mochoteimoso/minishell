/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/19 20:22:41 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void printer(t_shell *mini)
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
		printf("fd_in: %d\n", mini->cmds[i]->fd_in);
		printf("fd_out: %d\n", mini->cmds[i]->fd_out);
		printf("|*************************************************|");
		printf("\n");
		i++;
	}
}

static int	init_shell(t_shell *mini, char **envp)
{
	mini->cmds = NULL;
	mini->env = list_env(envp);
	if (!mini->env)
	{
		cleaner(mini);
		free(mini);
		return (1);
	}
	mini->pending = copy_env(envp);
	if (!mini->pending)
	{
		cleaner(mini);
		free(mini);
		return (1);
	}
	to_alphabetical(mini->pending);
	mini->cmd_count = 0;
	mini->prev_pipe[0] = -1;
	mini->prev_pipe[1] = -1;
	mini->pids = NULL;
	mini->exit_stat = 0;
	return (0);
}

static int	is_this_empty(char *input)
{
	while (*input)
	{
		if (!ft_isspace(*input))
			return (0);
		input++;
	}
	return (1);
}

static int user_prompt(t_shell *mini)
{
	char	*input;
	init_sig();
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
			break ;
		if (input && *input)
		{
			if (is_this_empty(input))
			{
				free(input);
				continue;
			}
			add_history(input);
		}
		if (parse_and_validate_input(input, mini))
			error("ALL IS BROKE!!\n");
		printer(mini);
		if (execute_pipeline(mini))
			error("ALL IS TERRIBLY BROKEN\n");
	}
	return (0);
}

static int	activate_shell(char **envp)
{
	t_shell	*mini;

	mini = malloc(sizeof(t_shell));
	if (!mini)
	{
		error("Malloc failed\n");
		return (1);
	}
	if (init_shell(mini, envp))
		return (1);
	user_prompt(mini);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc != 1)
	{
		printf("Minishell doesn't take arguments\n");
		return (1);
	}
	return(activate_shell(envp));
}
