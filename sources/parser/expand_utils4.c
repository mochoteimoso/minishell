/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:34:02 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 11:14:44 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	just_a_quest(char *str, char *name, int *indx, t_expand *arg);

void	just_a_quest(char *str, char *name, int *indx, t_expand *arg)
{
	if (str[arg->i] == '?')
	{
		if (*indx < (int) sizeof(name) - 1)
			name[(*indx)++] = str[arg->i++];
	}
	name[*indx] = '\0';
}
