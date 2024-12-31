/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:34:02 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 15:00:56 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	just_a_quest(char *str, char *name, int *indx, t_expand *arg);
void	we_need_name(t_expand *arg, char *str, char *name, int *indx);

void	just_a_quest(char *str, char *name, int *indx, t_expand *arg)
{
	if (str[arg->i] == '?')
	{
		if (*indx < (int) sizeof(name) - 1)
			name[(*indx)++] = str[arg->i++];
	}
	name[*indx] = '\0';
}

void	we_need_name(t_expand *arg, char *str, char *name, int *indx)
{
	while (str[arg->i] && (ft_isalnum(str[arg->i]) || str[arg->i] == '_'))
	{
		if (*indx < (int) sizeof(name) - 1)
			name[(*indx)++] = str[arg->i++];
	}
	name[*indx] = '\0';
}
