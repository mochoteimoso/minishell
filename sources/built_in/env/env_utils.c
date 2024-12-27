/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:08:35 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/27 15:03:53 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	set_name(t_env *node, char *name)
{
	(void)name;
	node->name = ft_strdup(name);
	if (!node->name)
		return (1);
	return (0);
}

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
