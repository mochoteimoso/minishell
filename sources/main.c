/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 13:46:36 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			main(int argc, char **argv, char **envp);
static int	activate_shell(int status, char **envp);
static int	init_shell(t_shell *mini, char **envp);
static int	user_prompt(t_shell *mini, int status);
static int	handle_input(t_shell *mini, char *input);

int	g_sig = 0;

int	main(int argc, char **argv, char **envp)
{
	int	status;

	status = 0;
	(void)argv;
	if (argc != 1)
	{
		printf("Minishell doesn't take arguments\n");
		return (1);
	}
	return (activate_shell(status, envp));
}

static int	activate_shell(int status, char **envp)
{
	t_shell	*mini;

	mini = ft_calloc(1, sizeof(t_shell));
	if (!mini)
	{
		ft_putendl_fd("mini struct malloc failed", 2);
		status = 1;
		return (status);
	}
	if (init_shell(mini, envp))
	{
		status = 1;
		return (status);
	}
	status = user_prompt(mini, status);
	clean_env(mini->env, mini->pending);
	free(mini);
	return (status);
}

static int	init_shell(t_shell *mini, char **envp)
{
	mini->env = list_env(envp);
	if (!mini->env)
	{
		mini_cleaner(mini);
		free(mini);
		ft_putendl_fd("failed to create envp", 2);
		return (1);
	}
	mini->pending = copy_env(envp);
	if (!mini->pending)
	{
		mini_cleaner(mini);
		free(mini);
		ft_putendl_fd("pending list malloc failed", 2);
		return (1);
	}
	to_alphabetical(mini->pending);
	mini->cmds = NULL;
	mini->cmd_count = 0;
	mini->stdin_saved = -1;
	mini->stdout_saved = -1;
	mini->pids = NULL;
	mini->pipes = NULL;
	mini->exit_stat = 0;
	return (0);
}

static int	user_prompt(t_shell *mini, int status)
{
	char	*input;

	while (1)
	{
		if (isatty(fileno(stdin)))
			init_sig();
		input = readline("minishell> ");
		if (input == NULL)
			break ;
		if (is_this_empty(input))
		{
			free(input);
			continue ;
		}
		if (input && *input)
		{
			if (handle_input(mini, input))
				continue ;
		}
	}
	status = mini->exit_stat;
	return (status);
}

static int	handle_input(t_shell *mini, char *input)
{
	add_history(input);
	if (parse_and_validate_input(&input, mini))
	{
		free(input);
		return (1);
	}
	execute_pipeline(mini);
	free(input);
	return (0);
}

// void	printer(t_shell *mini)
// {
// 	int i = 0;

// 	while (mini->cmds[i])
// 	{
// 		printf("\n");
// 		printf("|*************************************************|\n");
// 		printf("Struct %d:\n", i);
// 		printf("segment: %s\n", mini->cmds[i]->seg);
// 		printf("command: %s\n", mini->cmds[i]->command);
// 		printf("command path: %s\n", mini->cmds[i]->cmd_path);
// 		if (mini->cmds[i]->args)
// 		{
// 			int j = 0;
// 			while (mini->cmds[i]->args[j])
// 			{
// 				printf("arg %d: {%s}\n", j, mini->cmds[i]->args[j]);
// 				j++;
// 			}
// 		}
// 		t_redir *redir = mini->cmds[i]->redir_head;
// 		int redir_index = 0;
// 		while (redir)
// 		{
// 			printf("Redir\n");
// 			printf("Redirection %d - type: %d\n", redir_index, redir->type);
// 			printf("Redirection %d - file: %s\n", redir_index, redir->file);
// 			printf("Redirection %d - delimiter: %s\n", redir_index, redir->delimiter);
// 			printf("Redirection %d - heredoc file: %s\n", redir_index, redir->heredoc_name);
// 			printf("Redirection %d - expand or not: %d\n", redir_index, redir->expand);
// 			redir = redir->next;
// 			redir_index++;
// 		}
// 		//printf("fd_in: %d\n", mini->cmds[i]->fd_in);
// 		//printf("fd_out: %d\n", mini->cmds[i]->fd_out);
// 		printf("|*************************************************|");
// 		printf("\n");
// 		i++;
// 	}
// }
