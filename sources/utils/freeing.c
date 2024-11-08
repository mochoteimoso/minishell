/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:13:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/08 13:46:59 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleaner(t_env *ll, char **array)
{
	t_env *temp;

	if (array)
		ft_free_array(array);
	while (ll != NULL)
	{
		temp = ll;
		free(ll->name);
		free(ll->value);
		ll = ll->next;
		free(temp);
	}
	//free(ll);
}

void	error(char *str)
{
	ft_putstr_fd(str, 2);
}
