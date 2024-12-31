/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:06:03 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 17:05:32 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void		ft_env_lstadd_back(t_env **lst, t_env *new);
t_env		*create_node(void);
t_env		*add_node(char *env);
static int	check_alloc_and_free(void *ptr, void *free_ptr1, void *free_ptr2);

/**
 * ft_env_lstadd_back - Adds a new node to the end
 * of the environment variable list.
 *
 * @lst: Pointer to the head of the environment variable list.
 * @new: The new node to add to the list.
 *
 * If the list is empty, the new node becomes the head.
 * If the list is not empty, traverses the list and appends the new node.
 */
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

/**
 * create_node - Creates a new environment variable node.
 *
 * Allocates memory for a new `t_env` node and initializes its fields (`name`,
 * `value`, and `next`) to NULL.
 *
 * Returns:
 * - A pointer to the newly created node.
 * - NULL if memory allocation fails.
 */
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

/**
 * add_node - Creates a new environment variable node from a string.
 *
 * @env: Environment variable string in the format "NAME=VALUE".
 *
 * Parses the `env` string to extract the name and value, then creates a new
 * node and populates it using `fill_node`. Handles memory allocation and cleanup
 * in case of errors.
 *
 * Returns:
 * - A pointer to the newly created node on success.
 * - NULL if any memory allocation fails or if the input format is invalid.
 */
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

/**
 * check_alloc_and_free - Checks memory allocation
 * and frees resources on failure.
 *
 * @ptr: Pointer to the memory allocation to check.
 * @free_ptr1: Pointer to the first resource to free on failure (can be NULL).
 * @free_ptr2: Pointer to the second resource to free on failure (can be NULL).
 *
 * If `ptr` is NULL, frees the provided resources and returns an error code.
 *
 * Returns:
 * - 1 if `ptr` is NULL (indicating allocation failure).
 * - 0 otherwise.
 */
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
