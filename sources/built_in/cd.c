/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/15 10:46:43 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_pwd(t_env *env, char *wd, char **oldpwd, int n)
{
	int	flg = 0;
	while (env)
	{
		if (ft_strcmp(env->name, "OLDPWD") == 0)
		{
			if (n)
				*oldpwd = ft_strdup(env->value);
			else
				env->value = ft_strdup(*oldpwd);
			flg += 1;
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

static void	old_pwd(t_shell *mini)
{
	char	*temp;
	t_env	*env;
	char	*pwd;
	char	*oldpwd;

	env = mini->env;
	pwd = getcwd(NULL, 0);
	oldpwd = ft_calloc(1, sizeof(char *));
	temp = ft_calloc(1, sizeof(char *));
	update_pwd(mini->env, pwd, &oldpwd, 1);
	temp = ft_strdup(oldpwd);
	if (!oldpwd)
		error("No OLDPWD set\n");
	chdir(temp);
	free(oldpwd);
	free(pwd);
}

static void	to_path(t_shell *mini, char *path)
{
	char	*cwd;
	char	*oldpwd;
	t_env	*env;

	env = mini->env;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return ;
	chdir(path);
	cwd = getcwd(NULL, 0);
	update_pwd(env, cwd, &oldpwd, 0);
	free(cwd);
	free(oldpwd);
}

/*Changes the current directory. Accepts a relative or absolute path as an argument.*/

int	built_cd(t_shell *mini, t_cmd *cmd)
{
	const char	*path;
	char		*cwd;


	if (ft_array_len(cmd->args) > 2)
		error("Too many arguments\n");
	cwd = getcwd(NULL, 0);
	if (!cwd)
		error("Malloc fail\n");
	if (!cmd->args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: no HOME\n");
			return (1);
		}
		chdir(path);
		free(cwd);
		return (0);
	}
	else if (cmd->args[1][0] == '-')
		old_pwd(mini);
	else
		to_path(mini, cmd->args[1]);
	free(cwd);
	return (0);
}
