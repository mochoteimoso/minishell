/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/03 18:08:19 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			main(int argc, char **argv, char **envp);
static int	activate_shell(int status, char **envp);
static int	init_shell(t_shell *mini, char **envp);
static int	user_prompt(t_shell *mini, int status);
static int	handle_input(t_shell *mini, char *input);

int	g_sig = 0;

/**
 * main - Entry point for the minishell program.
 * Validates arguments and starts the shell activation process.
 */
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

/**
 * activate_shell - Initializes and manages the main lifecycle of the shell.
 * Sets up the shell's environment and begins the user input loop.
 *
 * @status: Exit status of the shell.
 * @envp: Environment variables passed to the shell.
 *
 * Returns the final exit status of the shell.
 */
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

/**
 * init_shell - Prepares the shell's internal state and environment.
 * Initializes the environment list, pending list alphabetical order,
 * and basic shell variables.
 *
 * @mini: Pointer to the shell structure to be initialized.
 * @envp: Environment variables passed to the shell.
 *
 * Returns 0 on success, or 1 if initialization fails.
 */
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

/**
 * user_prompt - Main loop for handling user input.
 * Continuously reads and processes user commands until the shell exits.
 *
 * @mini: Pointer to the shell structure containing state and configurations.
 * @status: Current status of the shell process.
 *
 * Returns the final exit status of the shell.
 */
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

/**
 * handle_input - Processes and executes a single user input.
 * Adds input to history, validates and parses commands,
 * and executes the pipeline.
 *
 * @mini: Pointer to the shell structure.
 * @input: The user input string to be processed.
 *
 * Returns 0 on successful execution, or 1 on validation or execution error.
 */
static int	handle_input(t_shell *mini, char *input)
{
	add_history(input);
	if (parse_and_validate_input(&input, mini))
	{
		free(input);
		return (1);
	}
	free(input);
	execute_pipeline(mini);
	return (0);
}
