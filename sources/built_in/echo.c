/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:10:11 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/13 15:23:34 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int	no_new_line(char *arg)
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

/*Prints the given arguments to the standard output.
Supports the -n flag to suppress the newline at the end.*/

int	built_echo(t_cmd *cmd)
{
	int	nl;
	int	i;

	nl = 1;
	i = 1;
	if (cmd->args[1] && cmd->args[1][1] && (cmd->args[1][0] == '-') && (no_new_line(cmd->args[1]) == 1))
	{
		nl = 0;
		i++;
		while ((cmd->args[i][0] == '-') && no_new_line(cmd->args[i]))
			i++;
	}
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
