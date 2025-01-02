/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:10:11 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:52:56 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			built_echo(t_cmd *cmd);
static int	nl_arg(t_cmd *cmd, int *nl, int i);
static int	no_new_line(char *arg);

/**
 * built_echo - Handles the "echo" command for the minishell.
 *
 * @cmd: Pointer to the command structure containing arguments for "echo".
 *
 * Processes arguments to print them to standard output.
 * If the first argument is "-n", it suppresses the trailing newline.
 * Prints arguments separated by a space and adds a newline unless suppressed.
 *
 * Returns:
 * - Always returns 0 to indicate successful execution.
 */
int	built_echo(t_cmd *cmd)
{
	int	nl;
	int	i;

	nl = 1;
	i = 1;
	if (cmd->args[1] && cmd->args[1][1] && (cmd->args[1][0] == '-')
		&& (no_new_line(cmd->args[1]) == 1))
		i = nl_arg(cmd, &nl, i);
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (nl)
		printf("\n");
	return (0);
}

/**
 * nl_arg - Parses the "-n" option for the "echo" command.
 *
 * @cmd: Pointer to the command structure containing arguments for "echo".
 * @nl: Pointer to an integer that determines whether to print a newline.
 * @i: Current argument index.
 *
 * Updates `nl` to 0 if the "-n" option is detected
 * and advances the argument index.
 * Continues processing subsequent "-n" options if present.
 *
 * Returns:
 * - The updated argument index after processing "-n" options.
 */
static int	nl_arg(t_cmd *cmd, int *nl, int i)
{
	if (cmd->args[1] && cmd->args[1][1] && (cmd->args[1][0] == '-')
		&& (no_new_line(cmd->args[1]) == 1))
	{
		*nl = 0;
		i++;
		if (cmd->args[1][0])
		{
			while (cmd->args[i] && (cmd->args[i][0] == '-')
				&& no_new_line(cmd->args[i]))
				i++;
		}
	}
	return (i);
}

/**
 * no_new_line - Checks if an argument matches the "-n" option for "echo".
 *
 * @arg: The argument string to check.
 *
 * Verifies that the argument starts with a '-'
 * and consists only of 'n' characters.
 *
 * Returns:
 * - 1 if the argument matches the "-n" option.
 * - 0 otherwise.
 */
static int	no_new_line(char *arg)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
