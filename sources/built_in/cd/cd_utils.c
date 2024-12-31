/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:40:06 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 10:37:52 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	get_oldpwd(t_env *env, char **pwd);
int	update_env_value(t_env *env, char *new_value);
int	update_pwd(t_env *env, char *wd, char **oldpwd);
int	handle_update_pwd(t_shell *mini, char *pwd, char *oldpwd);

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

int	update_env_value(t_env *env, char *new_value)
{
	char	*temp;

	temp = ft_strdup(new_value);
	if (!temp)
		return (1);
	free(env->value);
	env->value = temp;
	return (0);
}

int	update_pwd(t_env *env, char *wd, char **oldpwd)
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
			if (update_env_value(env, wd))
				return (1);
			flg += 1;
		}
		if (flg == 2)
			break ;
		env = env->next;
	}
	return (0);
}

int	handle_update_pwd(t_shell *mini, char *pwd, char *oldpwd)
{
	if (update_pwd(mini->env, pwd, &oldpwd))
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
