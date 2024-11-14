/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:58:12 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/14 15:32:01 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_value(t_env *env, char *name)
{
	t_env	*temp;

	temp = env;
	while (temp->next)
	{
		if (ft_strcmp(temp->name, name) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (name);
}

char	*expand_var(t_shell *mini, char *str)
{
	char	*expanded;
	char	*temp;
	char	name[100];
	char	*value;
	int		s;
	int		i;
	int		indx;

	i = 0;
	s = 0;
	expanded = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			temp = ft_strjoin(expanded, ft_strndup(&str[s], i -s));
			free(expanded);
			expanded = temp;
			i++;
			indx = 0;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				name[indx++] = str[i++];
			name[indx] = '\0';
			value = get_value(mini->env, name);
			if (value)
			{
				temp = ft_strjoin(expanded, value);
				free(expanded);
				expanded = temp;
			}
			s = i;
		}
		else if (str[i] == '~')
		{
			temp = ft_strjoin(expanded, ft_strndup(&str[s], i -s));
			free(expanded);
			expanded = temp;
			i++;
			value = get_value(mini->env, "HOME");
			if (value)
			{
				temp = ft_strjoin(expanded, value);
				free(expanded);
				expanded = temp;
			}
			s = i;
		}
		else
			i++;
	}
	temp = ft_strjoin(expanded, ft_strndup(&str[s], i - s));
	free(expanded);
	expanded = temp;
	return (expanded);
}

int	expand_or_not(t_shell *mini, t_cmd *cmd)
{
	int	i;
	int j;
	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		j = 0;
		while (cmd->args[i][j])
		{
			if (cmd->args[i][j] == '"' || cmd->args[i][j] == '$' || cmd->args[i][j] == '~')
			{
				cmd->args[i] = expand_var(mini, cmd->args[i]);
			}
			j++;
		}
		i++;
	}
	return (0);
}
