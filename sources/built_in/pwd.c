/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:10:30 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/30 11:35:04 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		built_pwd(t_shell *mini);
t_env	*find_pwd(t_env *env, char *name);

/*Prints the current working directory.*/

int	built_pwd(t_shell *mini)
{
	char	*cwd;
	t_env	*pwd;

	cwd = getcwd(NULL, 0);
	pwd = find_pwd(mini->env, "PWD");
	if (!cwd)
	{
		if (mini->env)
		{
			if (pwd && pwd->value)
			{
				printf("%s\n", pwd->value);
				return (0);
			}
		}
		ft_putendl_fd("pwd: cannot access current directory", 2);
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

t_env	*find_pwd(t_env *env, char *name)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}
