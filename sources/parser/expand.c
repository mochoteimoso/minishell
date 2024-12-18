/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:58:12 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/18 20:50:43 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		free(value);
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
	while (str[arg->i] && (ft_isalnum(str[arg->i])
		|| str[arg->i] == '_' || str[arg->i] == '?'))
	{
		if (indx < (int) sizeof(name) - 1)
			name[indx++] = str[arg->i++];
	}
	name[indx] = '\0';
	value = NULL;
	init_vdata(&data, expanded, temp, name);
	if (handle_value(mini, &data))
		return (-1);
	arg->name = ft_strdup(data.name);
	arg->start += ft_strlen(arg->name) + 1;
	free(arg->name);
	return (arg->start);
}

int	expand_variable(t_shell *mini, char *str, char **expanded, t_expand *arg)
{
	int	cont;
	arg->start = arg->i;
	cont = arg->start;
	if (str[arg->i] == '$')
		arg->i = oh_its_a_dollar(mini, str, expanded, arg);
	else if (str[arg->i] == '~')
		arg->i = tildes_home(mini, str, expanded, arg);
	if (str[cont + 1] == '?')
		arg->i = cont + 2;
	return (arg->i);
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

int	handle_expand(t_shell *mini, t_cmd **cmd)
{
	char		*expanded;
	int			i;
	t_expand	arg;

	i = 0;
	expanded = ft_strdup("");
	the_arg(&arg, i);
	while ((*cmd)->segment[arg.i])
	{
		if (ft_isspace((*cmd)->segment[arg.i]) && !arg.dbl && !arg.sgl)
		{
			expanded = ft_strjoin_char(expanded, (*cmd)->segment[arg.i]);
			arg.i++;
		}
		if ((*cmd)->segment[arg.i] == '\'')
		{
			expanded = ft_strjoin(expanded, "'");
			arg.i = segment_in_quotes(mini, (*cmd)->segment, arg.i, &arg);
			expanded = ft_strjoin(expanded, arg.value);
			expanded = ft_strjoin(expanded, "'");
		}
		else if ((*cmd)->segment[arg.i] == '"')
		{
			expanded = ft_strjoin(expanded, "\"");
			arg.i = segment_in_quotes(mini, (*cmd)->segment, arg.i, &arg);
			expanded = ft_strjoin(expanded, arg.value);
			expanded = ft_strjoin(expanded, "\"");
		}
		else
		{
			arg.i = segment_no_quotes(mini, *cmd, arg.i, &arg);
			expanded = ft_strjoin(expanded, arg.value);
		}
	}
	free((*cmd)->segment);
	(*cmd)->segment = expanded;
	return (0);
}
