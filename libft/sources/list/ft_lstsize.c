/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:12:06 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:58:17 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lstsize - Counts the number of nodes in a linked list.
 *
 * @lst: Pointer to the head of the list.
 *
 * Returns:
 * The number of nodes in the list.
 *
 * Details:
 * - Iterates through the list and counts the nodes.
 */
int	ft_lstsize(t_list *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
