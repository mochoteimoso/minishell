/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:40:06 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/20 10:34:41 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	get_oldpwd(t_env *env, char **pwd)
{
	while (env)
	{
		if (ft_strcmp(env->name, "OLDPWD") == 0)
		{
			printf("%s\n", env->value);
			*pwd = ft_strdup(env->value);
			if (!*pwd)
				return (1);
		}
		env = env->next;
	}
	return (0);
}

int	old_pwd(t_shell *mini, t_cmd *cmd)
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
