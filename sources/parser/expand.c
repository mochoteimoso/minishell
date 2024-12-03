/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:58:12 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/03 14:16:17 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	tildes_home(t_shell *mini, char *str, char **expanded, t_expand *arg)
{
	char	*temp;
	char	*temp2;
	char	*value;

	temp2 = ft_strndup(&str[arg->start], arg->i - arg->start);
	temp = ft_strjoin(*expanded, temp2);
	free(temp2);
	free(*expanded);
	*expanded = temp;
	arg->i++;
	value = get_value(mini->env, "HOME");
	if (value)
	{
		temp = ft_strjoin(*expanded, value);
		free(*expanded);
		*expanded = temp;
	}
	return (arg->i);
}

int	handle_new_expand(char *temp, char **expanded)
{
	if (new_expanded(temp, expanded) == -1)
	{
		free(temp);
		return (-1);
	}
	return (0);
}

int	oh_its_a_dollar(t_shell *mini, char *str, char **expanded, t_expand *arg)
{
	char	*temp;
	char	*value;
	char	name[100];
	int		indx;
	t_vdata	data;
	temp = ft_strndup(&str[arg->start], arg->i - arg->start);
	if (!temp)
		return (-1);
	if (handle_new_expand(temp, expanded))
		return (-1);
	arg->i++;
	indx = 0;
	while (str[arg->i] && (ft_isalnum(str[arg->i]) || str[arg->i] == '_' || str[arg->i] == '?'))
	{
		if (indx < (int) sizeof(name) - 1)
			name[indx++] = str[arg->i++];
	}
	name[indx] = '\0';
	value = NULL;
	init_vdata(&data, expanded, temp, name);
	if (handle_value(mini, &data))
		return (-1);
	arg->start = arg->i;
	return (arg->i);
}

int	expand_variable(t_shell *mini, char *str, char **expanded, t_expand *arg)
{
	int	cont;
	arg->start = arg->i;
	cont = arg->start;
	if (str[arg->i] == '$')
		arg->i = oh_its_a_dollar(mini, str, expanded, arg);
	if (str[arg->i] == '~')
		arg->i = tildes_home(mini, str, expanded, arg);
	if (str[cont + 1] == '?')
		arg->i = cont + 2;
	return (arg->i);
}
