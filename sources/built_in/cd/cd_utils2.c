/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:14:49 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 13:46:58 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	no_cwd(t_shell *mini, t_env *pwd, char **cwd);
int	handle_path(t_shell *mini, char *oldpwd, char *path);

int	no_cwd(t_shell *mini, t_env *pwd, char **cwd)
{
	pwd = find_pwd(mini->env, "PWD");
	if (pwd && pwd->value)
	{
		*cwd = ft_strdup(pwd->value);
		if (!(*cwd))
		{
			error(mini, "cd: malloc failed");
			return (1);
		}
	}
	else
	{
		error(mini, "getcwd: cannot access parent directories:\
				No such file or directory");
		return (1);
	}
	return (0);
}

int	handle_path(t_shell *mini, char *oldpwd, char *path)
{
	char	*newcwd;

	if (chdir(path) == -1)
	{
		ft_putendl_fd("cd: no such file or directory", 2);
		free(oldpwd);
		return (1);
	}
	newcwd = getcwd(NULL, 0);
	if (!newcwd)
		newcwd = ft_strdup(path);
	if (update_pwd(mini->env, newcwd, &oldpwd))
	{
		free(newcwd);
		free(oldpwd);
		return (1);
	}
	free(newcwd);
	free(oldpwd);
	return (0);
}
