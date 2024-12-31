/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:08:35 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 17:05:58 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			fill_node(t_env *node, char *name, char *value);
int			set_value(t_env *node, char *value);
static int	set_name(t_env *node, char *name);

/**
 * fill_node - Fills an environment variable node with a name and a value.
 *
 * @node: Pointer to the environment variable node.
 * @name: Name string to be set in the node.
 * @value: Value string to be set in the node.
 *
 * Sets the name and value of the node by calling `set_name` and `set_value`.
 * If either operation fails, cleans up the node and returns an error.
 *
 * Returns:
 * - 0 on success.
 * - 1 if memory allocation fails.
 */
int	fill_node(t_env *node, char *name, char *value)
{
	if (set_name(node, name) == 1)
	{
		clean_env(node, NULL);
		free(name);
		return (1);
	}
	if (set_value(node, value))
	{
		clean_env(node, NULL);
		free(name);
		return (1);
	}
	return (0);
}

/**
 * set_value - Sets the value of an environment variable node.
 *
 * @node: Pointer to the environment variable node.
 * @value: Value string to be set in the node.
 *
 * Allocates memory and assigns a copy of the `value` string
 * to the node's `value` field.
 * If the `value` is NULL, sets the `value` field to an empty string.
 *
 * Returns:
 * - 0 on success.
 * - 1 if memory allocation fails.
 */
int	set_value(t_env *node, char *value)
{
	if (value)
	{
		node->value = ft_strdup(value);
		if (!node->value)
			return (1);
	}
	else
	{
		node->value = ft_strdup("");
		if (!node->value)
			return (1);
	}
	return (0);
}

/**
 * set_name - Sets the name of an environment variable node.
 *
 * @node: Pointer to the environment variable node.
 * @name: Name string to be set in the node.
 *
 * Allocates memory and assigns a copy of the `name` string
 * to the node's `name` field.
 *
 * Returns:
 * - 0 on success.
 * - 1 if memory allocation fails.
 */
static int	set_name(t_env *node, char *name)
{
	node->name = ft_strdup(name);
	if (!node->name)
		return (1);
	return (0);
}
