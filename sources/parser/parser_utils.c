/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:13:39 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:55:15 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		no_args(t_cmd *cmd, int i);
bool	is_empty_command(t_cmd *cmd, int i);
int		add_char(char *str, t_expand *arg);

/**
 * no_args - Creates an argument array with only the command.
 *
 * @cmd: Pointer to the command structure.
 * @i: Current position in the command string.
 *
 * Allocates memory for an argument array containing the command as the sole
 * argument. Returns the current position in the string or -1 on failure.
 */
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

/**
 * is_empty_command - Checks if a command segment is empty or invalid.
 *
 * @cmd: Pointer to the command structure.
 * @i: Current position in the command string.
 *
 * Skips over whitespace and verifies if the command segment is empty or ends
 * with an invalid character (e.g., a pipe). Returns true if the command is
 * invalid or empty, false otherwise.
 */
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

/**
 * add_char - Appends a single character to an expandable string.
 *
 * @str: Input string containing the character to add.
 * @arg: Pointer to the expand structure holding the expandable string.
 *
 * Allocates and appends a character from `str` at the current position `arg->i`
 * to `arg->value`. Updates the position and ensures proper memory management.
 * Returns 0 on success or 1 on memory allocation failure.
 */
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
