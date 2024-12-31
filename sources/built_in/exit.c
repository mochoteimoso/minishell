/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:55:24 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 17:02:42 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			built_exit(t_shell *mini, t_cmd *cmd);
static void	not_all_num(t_shell *mini, t_cmd *cmd);
static int	isonlynum(char *str);

/**
 * built_exit - Handles the "exit" command for the minishell.
 *
 * @mini: Pointer to the shell structure containing state and resources.
 * @cmd: Pointer to the command structure containing arguments for "exit".
 *
 * Prints "exit" and processes the optional numeric
 * exit code from the first argument.
 * If the argument is not a valid numeric string,
 * it prints an error and exits with code 2.
 * If there are more than one arguments, it prints an error message
 * and returns 1 without exiting.
 * Cleans up resources before exiting.
 *
 * Returns:
 * - Does not return; calls `exit` to terminate the program.
 */
int	built_exit(t_shell *mini, t_cmd *cmd)
{
	int	ec;

	ft_putstr_fd("exit\n", 0);
	ec = 0;
	if (cmd->args[1])
	{
		if ((cmd->args[1] && !isonlynum(cmd->args[1]))
			|| (cmd->args[1] && cmd->args[1][0] == '\0'))
			not_all_num(mini, cmd);
		else if (cmd->args[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			mini->exit_stat = 1;
			return (1);
		}
		if (cmd->args[1])
			ec = ft_atoi(cmd->args[1]);
	}
	else
		ec = mini->exit_stat;
	mini_cleaner(mini);
	free(mini);
	rl_clear_history();
	exit(ec);
}

/**
 * not_all_num - Handles invalid numeric arguments for the "exit" command.
 *
 * @mini: Pointer to the shell structure containing state and resources.
 * @cmd: Pointer to the command structure containing the invalid argument.
 *
 * Prints an error message indicating that the argument is not a valid number.
 * Cleans up resources and exits with code 2.
 */
static void	not_all_num(t_shell *mini, t_cmd *cmd)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(cmd->args[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	mini_cleaner(mini);
	free(mini);
	rl_clear_history();
	exit(2);
}

/**
 * isonlynum - Checks if a string consists only of numeric characters.
 *
 * @str: The string to check.
 *
 * Allows an optional '+' or '-' at the beginning, followed by digits only.
 *
 * Returns:
 * - 1 if the string is a valid numeric representation.
 * - 0 if the string contains non-numeric characters.
 */
static int	isonlynum(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
