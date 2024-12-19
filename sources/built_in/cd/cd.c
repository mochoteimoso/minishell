/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/19 19:38:58 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	update_env_value(t_env *env, char *new_value)
{
	char	*temp;

	temp = ft_strdup(new_value);
	if (!temp)
		return (1);
	free(env->value);
	env->value = temp;
	return (0);
}

int	update_pwd(t_env *env, char *wd, char **oldpwd, int n)
{
	int	flg;

	flg = 0;
	while (env)
	{
		if (ft_strcmp(env->name, "OLDPWD") == 0
			&& update_env_value(env, *oldpwd))
			return (1);
		if (ft_strcmp(env->name, "PWD") == 0)
		{
			if (n)
			{
				wd = ft_strdup(env->value);
				if (!wd)
					return (1);
			}
			else if (update_env_value(env, wd))
				return (1);
			flg += 1;
		}
		if (flg == 2)
			break ;
		env = env->next;
	}
	return (0);
}

static int	to_path(t_shell *mini, char *path)
{
	char	*cwd;
	char	*oldpwd;
	t_env	*env;

	env = mini->env;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	if (chdir(path))
	{
		ft_putendl_fd("No such file or directory", 2);
		mini->exit_stat = 1;
		free(oldpwd);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (update_pwd(env, cwd, &oldpwd, 0))
	{
		free(cwd);
		free(oldpwd);
		return (1);
	}
	free(cwd);
	free(oldpwd);
	return (0);
}

static int	to_home(char *cwd)
{
	const char	*path;

	path = getenv("HOME");
	if (!path)
	{
		ft_putendl_fd("cd: no HOME", 2);
		free(cwd);
		return (1);
	}
	chdir(path);
	free(cwd);
	return (0);
}

/*Changes the current directory. Accepts a relative or absolute path 
as an argument.*/

int	built_cd(t_shell *mini, t_cmd *cmd)
{
	char	*cwd;

	if (ft_array_len(cmd->args) > 2)
	{
		error(mini, " too many arguments");
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		error(mini, "Malloc fail");
		return (1);
	}
	if (!cmd->args[1])
	{
		if (to_home(cwd))
			return (1);
		return (0);
	}
	else if (cmd->args[1][0] == '-')
	{
		if (old_pwd(mini, cmd))
			return (0);
	}
	else
	{
		if (to_path(mini, cmd->args[1]))
			return (1);
	}
	free(cwd);
	return (0);
}
