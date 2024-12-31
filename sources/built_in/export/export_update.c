/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:49:25 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 17:09:59 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			update_env(t_shell *mini, char *str);
int			update_pending(t_shell *mini, char *name, char *str);
int			handle_sign(t_shell *mini, char *str);
static int	not_on_the_list(t_shell *mini, char *str);
static int	set_new_value(t_env *temp, char *str);

/**
 * handle_sign - Adds or updates an environment variable in the shell.
 *
 * @mini: Pointer to the shell structure containing environment variables.
 * @str: The variable string to add or update.
 *
 * If the variable exists in the environment, it updates the value.
 * Otherwise, it adds a new variable to the environment
 * and updates pending variables.
 *
 * Returns:
 * - 0 on success.
 */
int	handle_sign(t_shell *mini, char *str)
{
	t_env	*new;

	if (update_env(mini, str) == 0)
	{
		new = add_node(str);
		ft_env_lstadd_back(&mini->env, new);
		update_env(mini, str);
	}
	return (0);
}

/**
 * update_env - Updates the value of an existing environment variable.
 *
 * @mini: Pointer to the shell structure containing environment variables.
 * @str: The variable string with a new value to update.
 *
 * Searches for the variable by its name in the environment list. If found,
 * updates its value and reflects the change in the pending list. If not found,
 * does nothing.
 *
 * Returns:
 * - 1 if the variable was updated successfully.
 * - 0 if the variable was not found.
 */
int	update_env(t_shell *mini, char *str)
{
	t_env	*temp;
	int		len;

	len = 0;
	temp = mini->env;
	while (str[len] != '=')
		len++;
	while (temp)
	{
		if ((len == (int)ft_strlen(temp->name))
			&& (ft_strncmp(temp->name, str, len) == 0))
		{
			set_new_value(temp, str);
			update_pending(mini, temp->name, str);
			to_alphabetical(mini->pending);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

/**
 * update_pending - Updates the value of a variable in the pending list.
 *
 * @mini: Pointer to the shell structure containing the pending list.
 * @name: The name of the variable to update.
 * @str: The full variable string to update.
 *
 * Searches for the variable in the pending list. If found, updates its value.
 * If not found, adds the variable to the pending list.
 *
 * Returns:
 * - 0 on success.
 * - 1 if there is a memory allocation error.
 */
int	update_pending(t_shell *mini, char *name, char *str)
{
	int		len;
	int		n;

	len = 0;
	while (str[len] != '=' && str[len])
		len++;
	n = 0;
	while (mini->pending[n] != NULL)
	{
		if (name && (len == (int)ft_strlen(name))
			&& (ft_strncmp(mini->pending[n], str, len) == 0))
		{
			free(mini->pending[n]);
			mini->pending[n] = ft_strdup(str);
			return (0);
		}
		n++;
	}
	if (not_on_the_list(mini, str))
		return (1);
	return (0);
}

/**
 * not_on_the_list - Adds a new variable to the pending list if it doesn't exist.
 *
 * @mini: Pointer to the shell structure containing the pending list.
 * @str: The variable string to add.
 *
 * Expands the pending list and appends the new variable.
 * Ensures null termination of the list.
 *
 * Returns:
 * - 0 on success.
 * - 1 if there is a memory allocation error.
 */
static int	not_on_the_list(t_shell *mini, char *str)
{
	size_t	i;
	char	**temp;

	i = 0;
	i = ft_array_len(mini->pending);
	temp = ft_realloc(mini->pending, i * sizeof(char *),
			(i + 2) * sizeof(char *));
	if (!temp)
		return (1);
	mini->pending = temp;
	mini->pending[i] = ft_strdup(str);
	mini->pending[i + 1] = NULL;
	return (0);
}

/**
 * set_new_value - Sets a new value for an existing environment variable.
 *
 * @temp: Pointer to the environment variable node to update.
 * @str: The variable string containing the new value.
 *
 * Extracts the value from the variable string
 * and updates the `value` field of the
 * environment variable node. Frees the old value before updating.
 *
 * Returns:
 * - 0 on success.
 * - 1 if there is a memory allocation error or failure to set the value.
 */
static int	set_new_value(t_env *temp, char *str)
{
	char	*value;
	char	*sign;

	sign = ft_strchr(str, '=');
	value = strdup(sign + 1);
	free(temp->value);
	if (!value)
		return (free(temp), 1);
	if (set_value(temp, value))
	{
		clean_env(temp, NULL);
		return (1);
	}
	free(value);
	return (0);
}
