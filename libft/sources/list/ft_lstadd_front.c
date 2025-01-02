/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 13:56:25 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:49:01 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lstadd_front - Adds a new node at the beginning of a linked list.
 *
 * @lst: Double pointer to the head of the list.
 * @new: The new node to add at the beginning.
 *
 * Details:
 * - Links the new node to the current head of the list.
 * - Updates the head pointer to point to the new node.
 */
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}
