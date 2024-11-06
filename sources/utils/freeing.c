/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:13:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/06 16:13:35 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleaner(t_env *ll, char **array)
{
	t_env *temp;
	int i = 0;
	(void)array;

	while (ll != NULL)
	{
		++i;
		temp = ll;
		free(ll->name);
		free(ll->value);
		ll = ll->next;
		free(temp);
	}
	free(ll);


	
}

void	error(char *str)
{
	ft_putstr_fd(str, 2);
}
