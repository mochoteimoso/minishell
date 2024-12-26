/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:49:25 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/26 15:56:10 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			update_env(t_shell *mini, char *str);
int			update_pending(t_shell *mini, char *name, char *str);
int			handle_sign(t_shell *mini, char *str);
static int	not_on_the_list(t_shell *mini, char *str);
static int	set_new_value(t_env *temp, char *str);

int	handle_sign(t_shell *mini, char *str)
{
	t_env	*new;

	if (update_env(mini, str) == 0)
	{
		new = add_node(str);
		ft_env_lstadd_back(&mini->env, new);
		update_env(mini, str);
	}
	return (0);
}

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
			update_pending(mini, temp->name, str);
			to_alphabetical(mini->pending);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

int	update_pending(t_shell *mini, char *name, char *str)
{
	int		len;
	int		n;

	len = 0;
	while (str[len] != '=' && str[len])
		len++;
	n = 0;
	while (mini->pending[n] != NULL)
	{
		if (name && (len == (int)ft_strlen(name))
			&& (ft_strncmp(mini->pending[n], str, len) == 0))
		{
			free(mini->pending[n]);
			mini->pending[n] = ft_strdup(str);
			return (0);
		}
		n++;
	}
	if (not_on_the_list(mini, str))
		return (1);
	return (0);
}

static int	not_on_the_list(t_shell *mini, char *str)
{
	size_t	i;
	char	**temp;

	i = 0;
	i = ft_array_len(mini->pending);
	temp = ft_realloc(mini->pending, i * sizeof(char *),
			(i + 2) * sizeof(char *));
	if (!temp)
		return (1);
	mini->pending = temp;
	mini->pending[i] = ft_strdup(str);
	mini->pending[i + 1] = NULL;
	return (0);
}

static int	set_new_value(t_env *temp, char *str)
{
	char	*value;
	char	*sign;

	sign = ft_strchr(str, '=');
	value = strdup(sign + 1);
	free(temp->value);
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
