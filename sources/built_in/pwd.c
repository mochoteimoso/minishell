/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:10:30 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/05 11:48:04 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Prints the current working directory.*/

int	built_pwd(t_shell *sh)
{
	char	*cwd;

	if (!sh->envp)
		return (1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putendl_fd("pwd not found", 2);
		cwd = ft_strdup("lost");
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}



// {
// 	char	cwd[PATH_MAX];
// 	char	*str;
// 	str = getcwd(cwd, sizeof(cwd));
// 	if (str)
// 	{
// 		ft_putstr_fd(str, 1);
// 		printf("\n");
// 		return ;
// 	}
// 	else
// 	{
// 		ft_putstr_fd("cannot access directories: ", 2);
// 		return ;
// 	}
// }
