/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:10:30 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/13 12:05:09 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Prints the current working directory.*/

int	built_pwd(t_shell *mini)
{
	char	*cwd;

	if (!mini->env)
		return (1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putendl_fd("pwd not found", 2);
		cwd = ft_strdup("lost");
		if (!cwd)
			return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
