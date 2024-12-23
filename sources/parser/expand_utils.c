/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:44:51 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/23 18:48:26 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			we_have_dollar(t_shell *mini, t_expand *arg, char *str);
int			oh_a_dollar(t_shell *mini, char *str, char **expan, t_expand *arg);
static int	finalize_expand(t_shell *mini, t_vdata *data, t_expand *arg);
static int	handle_new_expand(char *temp, char **expan);
static void	init_vdata(t_vdata *data, char **expan, char *temp, char *name);

int	we_have_dollar(t_shell *mini, t_expand *arg, char *str)
{
	int		s_exp;
	char	*temp;
	char	*new_res;

	s_exp = arg->i;
	temp = ft_strdup("");
	if (!temp)
		return (-1);
	arg->start = arg->i;
	arg->i = oh_a_dollar(mini, str, &temp, arg);
	if (arg->i == -1)
	{
		free(temp);
		return (-1);
	}
	new_res = ft_strjoin(arg->value, temp);
	if (!new_res)
	{
		free(temp);
		return (-1);
	}
	free(arg->value);
	free(temp);
	arg->value = new_res;
	return (0);
}

int	oh_a_dollar(t_shell *mini, char *str, char **expan, t_expand *arg)
{
	char	*temp;
	char	name[100];
	int		indx;
	t_vdata	data;

	temp = ft_strndup(&str[arg->start], arg->i - arg->start);
	if (!temp)
		return (-1);
	if (handle_new_expand(temp, expan))
		return (-1);
	arg->i++;
	indx = 0;
	while (str[arg->i] && (ft_isalnum(str[arg->i])
			|| str[arg->i] == '_' || str[arg->i] == '?'))
	{
		if (indx < (int) sizeof(name) - 1)
			name[indx++] = str[arg->i++];
	}
	name[indx] = '\0';
	init_vdata(&data, expan, temp, name);
	if (finalize_expand(mini, &data, arg))
		return (-1);
	return (arg->start);
}

static int	finalize_expand(t_shell *mini, t_vdata *data, t_expand *arg)
{
	if (handle_value(mini, data))
		return (-1);
	arg->name = ft_strdup(data->name);
	arg->start += ft_strlen(arg->name) + 1;
	free(arg->name);
	return (0);
}

static int	handle_new_expand(char *temp, char **expan)
{
	char	*new_expanded;

	new_expanded = ft_strjoin(*expan, temp);
	if (!new_expanded)
	{
		ft_putendl_fd("malloc failed", 2);
		return (-1);
	}
	free(temp);
	free(*expan);
	*expan = new_expanded;
	return (0);
}

static void	init_vdata(t_vdata *data, char **expan, char *temp, char *name)
{
	data->value = NULL;
	data->expan = expan;
	data->temp = temp;
	data->name = name;
}
