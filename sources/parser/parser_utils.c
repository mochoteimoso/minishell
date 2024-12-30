/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:13:39 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/30 11:45:11 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		no_args(t_cmd *cmd, int i);
bool	is_empty_command(t_cmd *cmd, int i);
int		add_char(char *str, t_expand *arg);

int	no_args(t_cmd *cmd, int i)
{
	cmd->args = ft_calloc(2, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[0] = ft_strdup(cmd->command);
	cmd->a_num = 1;
	if (!cmd->args)
		return (-1);
	cmd->args[1] = NULL;
	return (i);
}

bool	is_empty_command(t_cmd *cmd, int i)
{
	int	len;

	len = ft_strlen(cmd->seg);
	while (cmd->seg[i] && ft_isspace(cmd->seg[i]))
		i++;
	if (i != len - 1)
	{
		if (!cmd->seg[i] || cmd->seg[i] == '|')
			return (true);
	}
	return (false);
}

int	add_char(char *str, t_expand *arg)
{
	char	*temp;
	char	*temp2;
	char	*temp3;

	temp2 = ft_strndup(&str[arg->i], 1);
	if (!temp2)
		return (1);
	temp3 = ft_strdup(arg->value);
	if (!temp3)
	{
		free(temp2);
		return (1);
	}
	temp = ft_strjoin(temp3, temp2);
	free(temp2);
	free(temp3);
	if (!temp)
		return (1);
	free(arg->value);
	arg->value = temp;
	arg->i++;
	return (0);
}
