/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:06:03 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/04 17:52:31 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_env_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_env	*create_node(void)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = NULL;
	node->value = NULL;
	node->next = NULL;
	return (node);
}

static int	set_name(t_env *node, char *name)
{
	node->name = ft_strdup(name);
	if (!node->name)
		return (1);
	return (0);
}

static int	set_value(t_env *node, char *value)
{
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = ft_strdup("");
	if (!node->value)
		return (1);
	return (0);
}

static int	fill_node(t_env *node, char *name, char *value)
{
	if (set_name(node, name) == 1)
	{
		clean_env(node, NULL);
		free(name);
		return (1);
	}
	if (set_value(node, value))
	{
		clean_env(node, NULL);
		free(name);
		free(value);
		return (1);
	}
	return (0);
}

t_env	*add_node(char *env)
{
	t_env	*node;
	char	*name;
	char	*value;
	char	*sign;

	node = create_node();
	sign = ft_strchr(env, '=');
	name = ft_substr(env, 0, (sign - env));
	if (!name)
		return (NULL);
	value = ft_strdup(sign + 1);
	if (!value)
	{
		free(name);
		return (NULL);
	}
	if (fill_node(node, name, value))
		return (NULL);
	node->next = NULL;
	free(name);
	free(value);
	return (node);
}

