/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:13:39 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/12 17:48:57 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		no_args(t_cmd *cmd, int i);
bool	is_empty_command(t_cmd *cmd, int i);

int	no_args(t_cmd *cmd, int i)
{
	cmd->args = ft_calloc(2, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[0] = ft_strdup(cmd->command);
	if (!cmd->args)
		return (-1);
	cmd->args[1] = NULL;
	return (i);
}

bool	is_empty_command(t_cmd *cmd, int i)
{
	int	len;

	len = ft_strlen(cmd->segment);
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	if (i != len - 1)
	{
		if (!cmd->segment[i] || cmd->segment[i] == '|')
		{
			//t_putendl_fd("syntax error: expected a command", 2);
			return (true);
		}
	}
	return (false);
}
