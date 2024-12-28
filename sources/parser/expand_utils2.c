/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:18:09 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/28 20:03:11 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			tildes_home(t_shell *mini, char *str, char **expan, t_expand *arg);
int			handle_value(t_shell *mini, t_vdata *data);
char		*get_value(t_env *env, char *name);
char		*ft_strjoin_char(char *str, char c);
static int	we_have_value(char *value, char *temp, char **expan);

int	tildes_home(t_shell *mini, char *str, char **expan, t_expand *arg)
{
	char	*temp;
	char	*temp2;
	char	*value;

	temp2 = ft_strndup(&str[arg->start], arg->i - arg->start);
	temp = ft_strjoin(*expan, temp2);
	free(temp2);
	free(*expan);
	*expan = temp;
	arg->i++;
	value = get_value(mini->env, "HOME");
	if (value)
	{
		temp = ft_strjoin(*expan, value);
		if (!temp)
		{
			free(value);
			return (-1);
		}
		free(*expan);
		*expan = temp;
		free(value);
	}
	return (arg->i);
}

int	handle_value(t_shell *mini, t_vdata *data)
{
	if (data->name[0] == '?')
	{
		data->value = ft_itoa(mini->exit_stat);
		if (we_have_value(data->value, data->temp, data->expan) == -1)
			return (1);
		return (0);
	}
	data->value = get_value(mini->env, data->name);
	if (data->value == (char *)-1)
		return (1);
	if (we_have_value(data->value, data->temp, data->expan) == -1)
		return (1);
	return (0);
}

char	*get_value(t_env *env, char *name)
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
	value = ft_strdup("");
	if (!value)
	{
		ft_putendl_fd("malloc fail", 2);
		return ((char *) -1);
	}
	return (value);
}

static int	we_have_value(char *value, char *temp, char **expan)
{
	temp = ft_strjoin(*expan, value);
	free(value);
	if (!temp)
	{
		ft_putendl_fd("malloc failed", 2);
		return (-1);
	}
	free(*expan);
	*expan = temp;
	return (0);
}

char	*ft_strjoin_char(char *str, char c)
{
	size_t	len;
	char	*new_str;
	size_t	i;

	len = 0;
	if (str != NULL)
		len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (new_str == NULL)
		return (NULL);
	i = 0;
	if (str != NULL)
	{
		while (str[i] != '\0')
		{
			new_str[i] = str[i];
			i++;
		}
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	return (new_str);
}
