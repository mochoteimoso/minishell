/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:53:40 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/04 13:46:54 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_in_exe(t_shell *mini, t_cmd *cmd);
int	is_this_builtin_cmd(t_cmd *cmd);

// Executes relevant builtin function

int	built_in_exe(t_shell *mini, t_cmd *cmd)
{
	if (ft_strcmp(cmd->command, "exit") == 0)
		built_exit(mini, cmd);
	else if (ft_strcmp(cmd->command, "cd") == 0)
		return (built_cd(mini, cmd));
	else if (ft_strcmp(cmd->command, "echo") == 0)
		return (built_echo(cmd));
	else if (ft_strcmp(cmd->command, "env") == 0)
		return (built_env(mini));
	else if (ft_strcmp(cmd->command, "pwd") == 0)
		return (built_pwd(mini));
	else if (ft_strcmp(cmd->command, "unset") == 0)
		return (built_unset(mini, cmd));
	else if (ft_strcmp(cmd->command, "export") == 0)
		return (built_export(mini, cmd));
	return (0);
}

// Checks whether the command is a builtin

int	is_this_builtin_cmd(t_cmd *cmd)
{
	if (ft_strcmp(cmd->command, "exit") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "export") == 0)
		return (1);
	return (0);
}
