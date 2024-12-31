/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:55:24 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 11:08:45 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			built_exit(t_shell *mini, t_cmd *cmd);
static void	not_all_num(t_shell *mini, t_cmd *cmd);
static int	isonlynum(char *str);

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
