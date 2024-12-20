/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:12:11 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/20 12:48:17 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_value(t_env *env, char *name)
{
	t_env	*temp;
	char	*value;

	temp = env;
	// printf("name: {%s}\n", name);
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

static int	we_have_value(char *value, char *temp, char **expanded)
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
	return (0);
}

int	handle_value(t_shell *mini, t_vdata *data)
{
	if (data->name[0] == '?')
	{
		data->value = ft_itoa(mini->exit_stat);
		if (we_have_value(data->value, data->temp, data->expanded) == -1)
			return (1);
		return (0);
	}
	data->value = get_value(mini->env, data->name);
	if (data->value == (char *)-1)
		return (1);
	if (we_have_value(data->value, data->temp, data->expanded) == -1)
		return (1);
	return (0);
}

void	init_vdata(t_vdata *data, char **expanded, char *temp, char *name)
{
	data->value = NULL;
	data->expanded = expanded;
	data->temp = temp;
	data->name = name;
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
