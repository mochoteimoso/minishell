/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:01:15 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 17:09:11 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			built_export(t_shell *mini, t_cmd *cmd);
static void	print_pending(t_shell *mini);
static int	parse_and_add(t_shell *mini, char *str);
static int	validate_variable(char *name);

/**
 * built_export - Handles the "export" built-in command in the minishell.
 *
 * @mini: Pointer to the shell structure containing environment
 * 		  and pending variables.
 * @cmd: Pointer to the command structure with arguments for "export".
 *
 * Handles exporting variables to the environment or pending list.
 * If no arguments are provided, it prints the current pending variables.
 * Validates and processes each argument to either
 * add it to the environment or pending list.
 *
 * Returns:
 * - 0 on success.
 * - 1 if an argument is not a valid identifier.
 * - 2 if an invalid option is provided.
 */
int	built_export(t_shell *mini, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (cmd->a_num == 1)
		print_pending(mini);
	if (cmd->a_num >= 2)
	{
		while (i < cmd->a_num)
		{
			if (cmd->args[i][0] && cmd->args[i][0] == '-')
			{
				ft_putendl_fd("Invalid option", 2);
				return (2);
			}
			if (parse_and_add(mini, cmd->args[i]))
			{
				ft_putendl_fd("not a valid identifier", 2);
				return (1);
			}
			i++;
		}
	}
	return (0);
}

/**
 * print_pending - Prints all variables in the pending list.
 *
 * @mini: Pointer to the shell structure containing the pending variables.
 *
 * Iterates through the pending list and prints each variable.
 */
static void	print_pending(t_shell *mini)
{
	int	i;

	i = 0;
	while (mini->pending[i])
	{
		printf("%s\n", mini->pending[i]);
		i++;
	}
}

/**
 * parse_and_add - Parses and adds a variable to the environment or pending list.
 *
 * @mini: Pointer to the shell structure containing environment
 * 		  and pending variables.
 * @str: The variable string to parse and add.
 *
 * Splits the variable string into a name
 * and value pair based on the '=' character.
 * If the '=' is absent, the variable is added to the pending list.
 * Otherwise, the variable is added to the environment.
 *
 * Returns:
 * - 0 on success.
 * - 1 if the variable is not valid.
 */
static int	parse_and_add(t_shell *mini, char *str)
{
	char	*sign;

	if (!validate_variable(str))
	{
		mini->exit_stat = 1;
		return (1);
	}
	sign = ft_strchr(str, '=');
	if (!sign)
	{
		update_pending(mini, str, str);
		return (0);
	}
	else if (sign)
	{
		handle_sign(mini, str);
		return (0);
	}
	mini->exit_stat = 1;
	return (1);
}

/**
 * validate_variable - Validates the syntax of a variable name.
 *
 * @name: The variable name to validate.
 *
 * Ensures that the name starts with an alphabetic character or '_'.
 * Verifies that the rest of the name contains only
 * alphanumeric characters or '_'.
 * Stops validation at the '=' character, if present.
 *
 * Returns:
 * - 1 if the variable name is valid.
 * - 0 if the variable name is invalid.
 */
static int	validate_variable(char *name)
{
	int	i;

	i = 0;
	if (!name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
