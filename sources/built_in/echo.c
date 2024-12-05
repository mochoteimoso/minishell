/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:10:11 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/05 18:31:28 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Prints the given arguments to the standard output.
Supports the -n flag to suppress the newline at the end.*/

int	built_echo(t_cmd *cmd)
{
	int	nl;
	int	i;

	nl = 1;
	i = 1;
	if (cmd->args[1] && (ft_strcmp(cmd->args[1], "-n") == 0) || ft_strcmp(cmd->args[1], "$?") == 0)
	{
		if (ft_strcmp(cmd->args[1], "-n") == 0)
		{
			nl = 0;
			i++;
		}
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
