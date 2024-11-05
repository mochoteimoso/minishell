/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:13:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/05 10:57:46 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleaner(t_env *node, char **temp)
{
	if (temp)
		ft_free_array(temp);
	if (node)
	{
		free(node->name);
		node->name = NULL;
		free(node->value);
		node->value = NULL;
	}
}

void	error(char *str)
{
	ft_putstr_fd(str, 2);
}
