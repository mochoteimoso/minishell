/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/06 11:19:18 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_pwd(t_env *env, t_env *wd, t_env *oldpwd, int n)
{
	int	flg = 0;
	while (env)
	{
		if (ft_strcmp(env->name, "OLDPWD") == 0)
		{
			if (n)
				oldpwd->value = env->value;
			else
				env->value = oldpwd->value;
			flg += 1;
		}
		if (ft_strcmp(env->name, "PWD") == 0)
		{
			if (n)
				wd->value = env->value;
			else
				env->value = wd->value;
			flg += 1;
		}
		if (flg == 2)
			break;
		env = env->next;
	}
}

static void	old_pwd(t_shell *sh)
{
	char	*temp;
	t_env	*env;
	t_env	*pwd;
	t_env	*oldpwd;

	env = sh->env;
	pwd = ft_calloc(1, sizeof(t_env));
	oldpwd = ft_calloc(1, sizeof(t_env));
	update_pwd(sh->env, pwd, oldpwd, 1);
	temp = oldpwd->value;
	if (!oldpwd)
		error("No OLDPWD set\n");
	chdir(temp);
	free(oldpwd);
	free(pwd);
}

static void	to_path(t_shell *sh, char *path)
{
	t_env	*cwd;
	t_env	*oldpwd;
	t_env	*env;

	env = sh->env;
	cwd = ft_calloc(1, sizeof(t_env));
	oldpwd = ft_calloc(1, sizeof(t_env));
	oldpwd->value = getcwd(NULL, 0);
	if (!oldpwd || !oldpwd->value)
		return ;
	chdir(path);
	cwd->value = getcwd(NULL, 0);
	update_pwd(env, cwd, oldpwd, 0);
}

/*Changes the current directory. Accepts a relative or absolute path as an argument.*/

int	built_cd(t_shell *sh, char **cmd)
{
	const char	*path;
	char		*cwd;
	//char		*oldpwd;

	if (ft_array_len(cmd) > 2)
		error("Too many arguments\n");
	cwd = getcwd(NULL, 0);
	//printf("cmd[0]: %s\ncmd[1]: %s\n", cmd[0], cmd[1]);
	if (!cwd)
		error("Malloc fail\n");
	if (!cmd[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: no HOME\n");
			return (1);
		}
		chdir(path);
		return (0);
	}
	else if (cmd[1][0] == '-')
		old_pwd(sh);
	else
		to_path(sh, cmd[1]);
	return (0);
}
