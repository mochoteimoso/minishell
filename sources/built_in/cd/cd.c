/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/30 19:17:45 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			built_cd(t_shell *mini, t_cmd *cmd);
static int	to_home(char *cwd);
static int	cd_choices(t_shell *mini, t_cmd *cmd, char *cwd);
static int	old_pwd(t_shell *mini, t_cmd *cmd);
static int	to_path(t_shell *mini, char *path);

/*Changes the current directory. Accepts a relative or absolute path
as an argument.*/
int	built_cd(t_shell *mini, t_cmd *cmd)
{
	char	*cwd;
	t_env	*pwd;

	if (ft_array_len(cmd->args) > 2)
	{
		error(mini, "too many arguments");
		return (1);
	}
	cwd = getcwd(NULL, 0);
	pwd = NULL;
	if (!cwd)
	{
		if (no_cwd(mini, pwd, &cwd))
			return (1);
	}
	if (cd_choices(mini, cmd, cwd))
	{
		free(cwd);
		return (1);
	}
	return (0);
}

static int	cd_choices(t_shell *mini, t_cmd *cmd, char *cwd)
{
	if (!cmd->args[1])
	{
		if (to_home(cwd))
			return (1);
		return (0);
	}
	else if (cmd->args[1][0] == '-' && cmd->args[1][1] == '\0')
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
	if (handle_update_pwd(mini, pwd, oldpwd))
		return (1);
	return (0);
}

static int	to_path(t_shell *mini, char *path)
{
	char	*cwd;
	char	*oldpwd;
	t_env	*pwd;

	cwd = getcwd(NULL, 0);
	oldpwd = NULL;
	pwd = find_pwd(mini->env, "PWD");
	if (cwd)
		oldpwd = ft_strdup(cwd);
	else
	{
		if (pwd)
			oldpwd = ft_strdup(pwd->value);
		else
			oldpwd = NULL;
	}
	free(cwd);
	if (!oldpwd)
		return (ft_putendl_fd("cd: failed to get OLDPWD", 2), 1);
	if (handle_path(mini, oldpwd, path))
		return (1);
	return (0);
}
