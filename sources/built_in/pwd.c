/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:10:30 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/31 10:48:41 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Prints the current working directory.*/

void	built_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*str;
	str = getcwd(cwd, sizeof(cwd));
	if (str)
	{
		ft_putstr_fd(str, 1);
		printf("\n");
		return ;
	}
	else
	{
		ft_putstr_fd("cannot access directories: ", 2);
		return ;
	}
}
