/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:10:11 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/06 14:21:04 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Prints the given arguments to the standard output.
Supports the -n flag to suppress the newline at the end.*/

int	built_echo(char **cmd)
{
	int	nl;
	int	i;

	nl = 1;
	i = 1;
	if (cmd[1] && ft_strcmp(cmd[1], "-n") == 0)
	{
		nl = 0;
		i++;
	}
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (nl)
		printf("\n");
	ft_free_array(cmd);
	return (0);
}
