/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:06:03 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/31 17:23:50 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_name(t_env *node, void *stuff)
{
	node->name = ft_strdup(stuff);
	if (!node->name)
		return (1);
	return (0);
}

int	set_value(t_env *node, void *stuff)
{
	char *sign;

	sign = ft_strchr(stuff, '=');
	if (sign)
		node->value = ft_strdup(sign + 1);
	else
		node->value = ft_strdup("");
	if (!node->value)
		return (1);
	return (0);
}
