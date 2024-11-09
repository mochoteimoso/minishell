/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:26:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/09 16:31:20 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_cmd_args(char *cmd_string, int i, t_shell *sh, int index);
int	count_args(char *cmd_string, int i);

// Retrieves command arguments from string and copies them to struct

int	handle_cmd_args(char *cmd_string, int i, t_shell *sh, int index)
{
	int		arg_length;
	char	*arg_start;
	char	args_count;
	int		arg_index;

	arg_index = 0;
	args_count = count_args(cmd_string, i);
	sh->cmds[index]->args = ft_calloc(args_count + 1, sizeof(char *));
	if (!sh->cmds[index]->args)
		return (-1);
	while (cmd_string[i] && ft_isspace(cmd_string[i]))
		i++;
	while (cmd_string[i] && arg_index <= args_count)
	{
		arg_start = &cmd_string[i];
		arg_length = 0;
		while (cmd_string[i] && !ft_isspace(cmd_string[i]) && !is_redirection(cmd_string, i))
		{
			arg_length++;
			i++;
		}
		sh->cmds[index]->args[arg_index] = ft_strndup(arg_start, arg_length);
		if (!sh->cmds[index]->args[arg_index])
		{
			printf("Failed to allocate memory for argument\n");
			return (-1);
		}
		arg_index++;
		while (cmd_string[i] && ft_isspace(cmd_string[i]))
			i++;
	}
	sh->cmds[index]->args[arg_index] = NULL;
	printf("index after handle_args: %d\n", i);
	return (i);
}

// Counts how many command arguments the string contains

int	count_args(char *cmd_string, int i)
{
	int	args_count;

	args_count = 0;
	while (cmd_string[i] && ft_isspace(cmd_string[i]))
		i++;
	while (cmd_string[i])
	{
		if (cmd_string[i] && !is_redirection(cmd_string, i))
		{
			args_count++;
			while (cmd_string[i] && !ft_isspace(cmd_string[i]) && !is_redirection(cmd_string, i))
				i++;
		}
		else if (is_redirection(cmd_string, i))
			break ;
		while (cmd_string[i] && ft_isspace(cmd_string[i]))
			i++;
	}
	printf("Argument count: %d\n", args_count);
	return (args_count);
}
