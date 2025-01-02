/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:55:16 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:58:03 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lstnew - Creates a new node for a linked list.
 *
 * @content: The content to store in the new node.
 *
 * Returns:
 * A pointer to the newly created node or NULL if allocation fails.
 *
 * Details:
 * - Allocates memory for a new node.
 * - Initializes the node's `content` and sets `next` to NULL.
 */
t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
