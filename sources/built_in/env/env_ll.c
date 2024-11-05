/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:06:03 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/01 14:37:37 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_env_lstadd_back(t_env *lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (!lst)
	{
		lst = new;
		return ;
	}
	temp = lst;
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
	return (node);
}

static int	set_name(t_env *node, void *stuff)
{
	node->name = ft_strdup(stuff);
	if (!node->name)
		return (1);
	return (0);
}

static int	set_value(t_env *node, void *stuff)
{
	char *sign;

	sign = ft_strchr(stuff, '=');
	if (sign)
		node->value = ft_strdup(sign + 1);
	else
		node->value = ft_strdup("");
	if (!node->value)
		return (1);
	return (0);
}

t_env	*add_node(void *stuff)
{
	t_env	*node;
	char	**temp;

	node = create_node();
	if (set_name(node, stuff) == 1)
	{
		free(node);
		return (NULL);
	}
	temp = ft_split(stuff, '=');
	if (!temp)
	{
		cleaner(node, NULL);
		return (NULL);
	}
	if (set_value(node, temp) == 1)
	{
		cleaner(node, temp);
		return (NULL);
	}
	node->next = NULL;
	ft_free_array(temp);
	return (node);
}
