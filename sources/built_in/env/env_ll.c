/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:06:03 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/15 09:54:09 by henbuska         ###   ########.fr       */
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

static t_env	*create_node(void)
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

t_env	*add_node(char *env)
{
	t_env	*node;
	char	**temp;

	node = create_node();
	temp = ft_split(env, '=');
	if (!temp || !temp[0])
	{
		clean_env(node, temp);
		return (NULL);
	}
	if (set_name(node, temp[0]) == 1)
	{
		free(node);
		return (NULL);
	}
	if (set_value(node, temp[1]) == 1)
	{
		clean_env(node, temp);
		return (NULL);
	}
	node->next = NULL;
	ft_free_array(temp);
	return (node);
}
