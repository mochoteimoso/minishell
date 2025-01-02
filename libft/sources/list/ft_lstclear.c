/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:42:28 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:51:13 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lstclear - Clears and frees all nodes in a linked list.
 *
 * @lst: Double pointer to the head of the list.
 * @del: Function to delete the content of a node.
 *
 * Details:
 * - Recursively clears the list by deleting each node's content using `del`.
 * - Frees the memory for each node and sets the list pointer to NULL.
 */
void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	if (!lst || !del || !(*lst))
		return ;
	ft_lstclear(&(*lst)->next, del);
	(del)((*lst)->content);
	free(*lst);
	*lst = NULL;
}
