/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:58:12 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/02 11:20:41 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_value(t_env *env, char *name)
{
	t_env	*temp;
	char	*value;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
		{
			value = ft_strdup(temp->value);
			if (!value)
			{
				ft_putendl_fd("malloc fail", 2);
				return ((char *) -1);
			}
			return (value);
		}
		temp = temp->next;
	}
	return (NULL);
}

int	new_expanded(char *temp, char **expanded)
{
	char	*new_expanded;
	new_expanded = ft_strjoin(*expanded, temp);
	if (!new_expanded)
	{
		ft_putendl_fd("malloc failed", 2);
		return (-1);
	}
	free(temp);
	free(*expanded);
	*expanded = new_expanded;
	return (0);
}

int we_have_value(char *value, char *temp, char **expanded)
{
	if (value)
	{
		temp = ft_strjoin(*expanded, value);
		free(value);
		if (!temp)
		{
			ft_putendl_fd("malloc failed", 2);
			return (-1);
		}
		free(*expanded);
		*expanded = temp;
	}
	return (0);
}

int	tildes_home(t_shell *mini, char *str, char **expanded, int i, int *s)
{
	char	*temp;
	char	*value;

	temp = ft_strjoin(*expanded, ft_strndup(&str[*s], i - *s));
	free(*expanded);
	*expanded = temp;
	i++;
	value = get_value(mini->env, "HOME");
	if (value)
	{
		temp = ft_strjoin(*expanded, value);
		free(*expanded);
		*expanded = temp;
	}
	return (i);
}

int	handle_value(t_shell *mini, char *value, char **expanded, char *temp, char *name)
{
	value = get_value(mini->env, name);
	if (value == (char *)-1)
		return (1);
	if (value)
	{
		if (we_have_value(value, temp, expanded) == -1)
			return (1);
	}
	return (0);
}

int handle_new_expand(char *temp, char **expanded)
{
	if (new_expanded(temp, expanded) == -1)
	{
		free(temp);
		return (-1);
	}
	return (0);
}

int oh_its_a_dollar(t_shell *mini, char *str, char **expanded, int i, int *s)
{
	char	*temp;
	char	*value;
	char	name[100];
	int		indx;

	temp = ft_strndup(&str[*s], i - *s);
	if (!temp)
		return (-1);
	if (handle_new_expand(temp, expanded))
		return (-1);
	i++;
	indx = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		if (indx < (int)sizeof(name) - 1)
			name[indx++] = str[i++];
	}
	name[indx] = '\0';
	value = NULL;
	if (handle_value(mini, value, expanded, temp, name))
		return (-1);
	*s = i;
	return (i);
}

int expand_variable(t_shell *mini, char *str, char **expanded, int i, int *s)
{
	if (str[i] == '$')
		i = oh_its_a_dollar(mini, str, expanded, i, s);
	if (str[i] == '~')
		i = tildes_home(mini, str, expanded, i, s);
	return (i);
}
