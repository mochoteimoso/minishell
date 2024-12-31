/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:06:03 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 11:06:49 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void		ft_env_lstadd_back(t_env **lst, t_env *new);
t_env		*create_node(void);
static int	check_alloc_and_free(void *ptr, void *free_ptr1, void *free_ptr2);
t_env		*add_node(char *env);

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

static int	check_alloc_and_free(void *ptr, void *free_ptr1, void *free_ptr2)
{
	if (!ptr)
	{
		if (free_ptr1)
			free(free_ptr1);
		if (free_ptr2)
			free(free_ptr2);
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
	if (!node || !sign)
		return (NULL);
	name = ft_substr(env, 0, (sign - env));
	if (check_alloc_and_free(name, node, NULL))
		return (NULL);
	value = ft_strdup(sign + 1);
	if (check_alloc_and_free(value, name, node))
		return (NULL);
	if (fill_node(node, name, value))
	{
		free(value);
		return (NULL);
	}
	node->next = NULL;
	free(name);
	free(value);
	return (node);
}
