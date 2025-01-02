/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:58:11 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:50:52 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lstadd_back - Adds a new node at the end of a linked list.
 *
 * @lst: Double pointer to the head of the list.
 * @new: The new node to add at the end.
 *
 * Details:
 * - Traverses the list to find the last node.
 * - Links the last node's `next` to the new node.
 * - If the list is empty, the new node becomes the head.
 */
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}
