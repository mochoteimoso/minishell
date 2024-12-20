/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:13:39 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/20 18:51:55 by nzharkev         ###   ########.fr       */
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

int	add_char(char *str, t_expand *arg)
{
	char	*temp;
	char	*temp2;
	char	*temp3;

	// printf("str[%d]: {%c}\n", arg->i, str[arg->i]);
	// printf("value: {%s}\n", arg->value);
	temp2 = ft_strndup(&str[arg->i], 1);
	if (!temp2)
		return (1);
	temp3 = ft_strdup(arg->value);
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
