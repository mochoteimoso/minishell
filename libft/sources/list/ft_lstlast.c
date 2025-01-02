/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:38:59 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:20:13 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lstlast - Returns the last node of a linked list.
 *
 * @lst: Pointer to the head of the list.
 *
 * Returns:
 * The last node in the list or NULL if the list is empty.
 *
 * Details:
 * - Traverses the list until the `next` pointer of a node is NULL.
 */
t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
