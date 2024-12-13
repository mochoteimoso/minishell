/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:55:24 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/13 20:10:42 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	built_exit(t_shell *mini, t_cmd *cmd)
{
	int	ec;

	ft_putstr_fd("exit\n", 1);    // should this be printed to stdout??
	ec = 0;
	if (cmd->args[1])
	{
		if ((cmd->args[1] && !isonlynum(cmd->args[1])) || (cmd->args[1] && cmd->args[1][0] == '\0'))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(cmd->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			cleaner(mini);
			free(mini);
			exit(2);
		}
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
	cleaner(mini);
	free(mini);
	exit(ec);
}
