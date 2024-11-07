/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:24:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/07 13:01:07 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	built_env(t_shell *mini)
{
	t_env *temp = mini->env;

	while (temp != NULL)
	{
		printf("%s=%s\n", temp->name, temp->value);
		temp = temp->next;
	}
	return (0);
}
