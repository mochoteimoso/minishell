/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 14:57:29 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/21 15:35:20 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	update_env(t_shell *mini, char *str)
{
	t_env	*temp;
	int		len;

	len = 0;
	temp = mini->env;
	while (str[len] != '=')
		len++;
	while (temp)
	{
		if ((len == (int)ft_strlen(temp->name))
			&& (ft_strncmp(temp->name, str, len) == 0))
		{
			set_new_value(temp, str);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

int	validate_variable(char *name)
{
	int	i;

	i = 0;
	if (!name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	set_new_value(t_env *temp, char *str)
{
	char	*value;
	char	*sign;

	sign = ft_strchr(str, '=');
	value = strdup(sign + 1);
	if (!value)
		return (free(temp), 1);
	if (set_value(temp, value))
	{
		clean_env(temp, NULL);
		return (1);
	}
	free(value);
	return (0);
}
