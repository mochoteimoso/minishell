/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:13:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/06 13:18:19 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleaner(t_env *ll, char **temp)
{
	if (temp)
		ft_free_array(temp);
	if (ll)
	{
		while (ll)
		{
			free(ll->name);
		//node->name = NULL;
			free(ll->value);
		//node->value = NULL;
			ll = ll->next;
		}
		free(ll);
	}
}

void	error(char *str)
{
	ft_putstr_fd(str, 2);
}
