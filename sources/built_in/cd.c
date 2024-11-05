/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/05 11:55:24 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	old_pwd(t_shell *sh)
{
	int		i;
	int		j;
	int		s;
	char	*temp;
	char	*new;

	i = 0;
	while (sh->envp[i] && ft_strncmp(sh->envp[i], "OLDPWD", 6) != 0)
		i++;
	if (!sh->envp[i])
		error("No OLDPWD set\n");
	s = 7;
	temp = (char *)malloc(sizeof(char) * (ft_strlen(sh->envp[i]) - s + 1));
	j = 0;
	while (sh->envp[i][s])
		temp[j++] = sh->envp[i][s++];
	chdir(temp);
	new = ft_strjoin("PWD", temp);
	s = 7;
	j= 0;
	while (temp[j])
		sh->envp[i][s++] = temp[j++];
	free(temp);
}

/*Changes the current directory. Accepts a relative or absolute path as an argument.*/

int	built_cd(t_shell *sh, char **cmd)
{
	const char	*path;
	char		*cwd;
	//char		*oldpwd;

	if (cmd[1] != NULL)
		error("Too many arguments\n");
	cwd = getcwd(NULL, 0);
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
	else if (cmd[1][0] == '-' && !cmd[2])
		old_pwd(sh);
	//else if (ft_strncmp(cmd[1], "..", 2))
	return (0);
}
