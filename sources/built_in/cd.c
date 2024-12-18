/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/16 16:04:20 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static void	update_pwd(t_env *env, char *wd, char **oldpwd, int n)
// {
// 	int	flg = 0;
// 	while (env)
// 	{
// 		if (ft_strcmp(env->name, "OLDPWD") == 0)
// 		{
// 			if (n)
// 				printf("%s\n", env->value);
// 			// *oldpwd = ft_strdup(env->value);
// 			env->value = ft_strdup(*oldpwd);
// 			flg += 1;
// 		}
// 		if (ft_strcmp(env->name, "PWD") == 0)
// 		{
// 			if (n)
// 				wd = ft_strdup(env->value);
// 			else
// 				env->value = ft_strdup(wd);
// 			flg += 1;
// 		}
// 		if (flg == 2)
// 			break;
// 		env = env->next;
// 	}
// }

static void	update_pwd(t_env *env, char *wd, char **oldpwd, int n)
{
	int flg;

	flg = 0;
	while (env)
	{
		if (ft_strcmp(env->name, "OLDPWD") == 0)
		{
			env->value = ft_strdup(*oldpwd);
		}
		if (ft_strcmp(env->name, "PWD") == 0)
		{
			if (n)
				wd = ft_strdup(env->value);
			else
				env->value = ft_strdup(wd);
			flg += 1;
		}
		if (flg == 2)
			break;
		env = env->next;
	}
}

static	int	get_oldpwd(t_env *env, char **pwd)
{
	while (env)
	{
		if (ft_strcmp(env->name, "OLDPWD") == 0)
		{
			printf("%s\n", env->value);
			*pwd = ft_strdup(env->value);
		}
		env = env->next;
	}
	return (0);
}

static int	old_pwd(t_shell *mini, t_cmd *cmd)
{
	t_env	*env;
	char	*pwd;
	char	*oldpwd;

	if (cmd->args[1][1])
		return (1);
	env = mini->env;
	pwd = NULL;
	get_oldpwd(env, &pwd);
	oldpwd = getcwd(NULL, 0);
	chdir(pwd);
	update_pwd(mini->env, pwd, &oldpwd, 1);
	if (!oldpwd)
		error(mini, "No OLDPWD set");
	free(oldpwd);
	free(pwd);
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
		return (1);
	}
	cwd = getcwd(NULL, 0);
	update_pwd(env, cwd, &oldpwd, 0);
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
		return (1);
	}
	chdir(path);
	free(cwd);
	return (0);
}

/*Changes the current directory. Accepts a relative or absolute path as an argument.*/
int	built_cd(t_shell *mini, t_cmd *cmd)
{
	// const char	*path;
	char		*cwd;

	if (ft_array_len(cmd->args) > 2)
	{
		error(mini, " too many arguments");
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		error(mini, "Malloc fail");
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
