/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:51:41 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/13 16:23:39 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	skip_whitespace(char *str, int i)
{
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

int	arg_in_quotes(char *str, int i, char **start, int *len)
{
	int	s;

	s = str[i];
	*start = &str[i];
	*len = 1;
	i++;
	while (str[i] && str[i] != s)
	{
		(*len)++;
		i++;
	}
	if (str[i] == s)
	{
		(*len)++;
		i++;
	}
	return (i);
}

int	arg_no_quotes(t_cmd *cmd, int i, char **start, int *len)
{
	*start = &cmd->segment[i];
	*len = 0;
	while (cmd->segment[i] && (!ft_isspace(cmd->segment[i]) || check_quotes(cmd->segment, i)) && !is_redirection(cmd, i))
	{
		(*len)++;
		i++;
	}
	return (i);
}

int	append_to_array(t_cmd *cmd, char *start, int len, int *index)
{
	cmd->args[*index] = ft_strndup(start, len);
	if (!cmd->args[*index])
	{
		printf("Failed to allocate memory for argument\n");
		return -1;
	}
	(*index)++;
	return (0);
}
