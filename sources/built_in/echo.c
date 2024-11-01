/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:10:11 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/01 14:14:10 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../includes/minishell.h"

// /*Prints the given arguments to the standard output.
// Supports the -n flag to suppress the newline at the end.*/

// void	built_echo(char **args)
// {
// 	int	nl;
// 	int	i;

// 	nl = 1;
// 	i = 1;
// 	if (args[1] && ft_strcmp(args[1], "-n") == 0)
// 	{
// 		nl = 0;
// 		i++;
// 	}
// 	while (args[i])
// 	{
// 		printf("%s", args[i]);
// 		if (args[i + 1])
// 			ft_putstr_fd(" ", 1);
// 		i++;
// 	}
// 	if (nl)
// 		ft_putstr_fd("\n", 1);
// }
