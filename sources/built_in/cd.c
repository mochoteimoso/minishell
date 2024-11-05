/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/05 10:24:35 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	old_pwd(t_shell *sh)
{
	int		i;
	int		j;
	int		s;
	char	*temp;
	char	*new;

	while (sh->envp[i] && ft_strncmp(sh->envp[i], "OLDPWD", 6) != 0)
		i++;
	if (!sh->envp[i])
		error("No OLDPWD set");
	s = 7;
	temp = (char *)malloc(sizeof(char) * (ft_strlen(envp[i]) - s + 1);
	j = 0;
	while (envp[i][s])
		temp[j++] = envp[i][s++];
	chdir(temp);
	new = ft_strjoin("PWD", temp);
	free(temp);
	
}

/*Changes the current directory. Accepts a relative or absolute path as an argument.*/

int	built_cd(t_shell *sh, char **cmd)
{
	const char	*path;
	char		*cwd;
	char		*oldpwd;

	if (cmd[2])
		error("Too many arguments");
	cwd = getcwd(NULL, 0);
	if (!cwd)
		error("Malloc fail");
	if (!cmd[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: no HOME\n");
			return (1);
		}
		return (0);
	}
	else if (cmd[1] == '-' && !cmd[2])
		old_pwd(sh);
	else if (ft_strncmp(cmd[1], "..", 2))
}
