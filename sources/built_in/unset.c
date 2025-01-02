/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:51:52 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:53:09 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			built_unset(t_shell *mini, t_cmd *cmd);
static int	unset_env(t_shell *mini, char *str);
static int	unset_pending(t_shell *mini, char *str);

/**
 * built_unset - Unsets environment variables or pending variables.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Pointer to the command structure containing arguments.
 *
 * Iterates through the command arguments, unsetting the specified environment
 * or pending variables. Checks for invalid options
 * (arguments starting with `-`).
 * Calls `unset_env` and `unset_pending` to remove the variables.
 *
 * Returns:
 * - 0 on success.
 * - 2 if an invalid option is detected.
 * - 1 if a variable is not found or another error occurs.
 */
int	built_unset(t_shell *mini, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (cmd->a_num >= 2)
	{
		while (i < cmd->a_num)
		{
			if (cmd->args[i][0] == '-')
			{
				ft_putendl_fd("Invalid option", 2);
				return (2);
			}
			if (unset_env(mini, cmd->args[i])
				|| unset_pending(mini, cmd->args[i]))
			{
				error(mini, "No such variable");
				return (1);
			}
			i++;
		}
	}
	return (0);
}

/**
 * unset_env - Removes an environment variable from the shell's environment list.
 *
 * @mini: Pointer to the shell structure containing the environment list.
 * @str: Name of the variable to unset.
 *
 * Traverses the linked list of environment variables to find and remove
 * the specified variable. Frees the memory associated with the variable node.
 *
 * Returns:
 * - 0 if the variable is successfully removed or not found.
 * - Non-zero values are not currently used.
 */
static int	unset_env(t_shell *mini, char *str)
{
	t_env	*cur;
	t_env	*prev;

	cur = mini->env;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->name, str) == 0)
		{
			if (!prev)
				mini->env = cur->next;
			else
				prev->next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return (0);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

/**
 * unset_pending - Removes a variable from the pending environment array.
 *
 * @mini: Pointer to the shell structure containing the pending variables array.
 * @str: Name of the variable to unset.
 *
 * Searches for the specified variable in the `mini->pending` array.
 * If found, removes the variable by shifting subsequent array elements
 * to close the gap and sets the last pointer to `NULL`.
 *
 * Returns: Always 0.
 */
static int	unset_pending(t_shell *mini, char *str)
{
	int	i;
	int	j;
	int	len;

	len = ft_strlen(str);
	i = 0;
	while (mini->pending && mini->pending[i])
	{
		if (ft_strncmp(mini->pending[i], str, len) == 0)
		{
			free(mini->pending[i]);
			break ;
		}
		i++;
	}
	j = i;
	while (mini->pending[j] && mini->pending[j + 1])
	{
		mini->pending[j] = mini->pending[j + 1];
		j++;
	}
	mini->pending[j] = NULL;
	return (0);
}
