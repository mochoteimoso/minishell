/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/26 15:30:35 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			built_cd(t_shell *mini, t_cmd *cmd);
static int	to_home(char *cwd);
static int	old_pwd(t_shell *mini, t_cmd *cmd);
static int	to_path(t_shell *mini, char *path);


/*Changes the current directory. Accepts a relative or absolute path
as an argument.*/
int	built_cd(t_shell *mini, t_cmd *cmd)
{
	char	*cwd;

	if (ft_array_len(cmd->args) > 2)
	{
		error(mini, "too many arguments");
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

static int	old_pwd(t_shell *mini, t_cmd *cmd)
{
	t_env	*env;
	char	*pwd;
	char	*oldpwd;

	if (cmd->args[1][1])
		return (1);
	env = mini->env;
	pwd = NULL;
	if (get_oldpwd(env, &pwd))
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		free(pwd);
		return (1);
	}
	chdir(pwd);
	if (update_pwd(mini->env, pwd, &oldpwd, 1))
	{
		free(pwd);
		free(oldpwd);
		return (1);
	}
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
