/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:01:18 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:19:27 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lstiter - Iterates through a linked list
 * 				and applies a function to each node's content.
 *
 * @lst: Pointer to the head of the list.
 * @f: Function to apply to each node's content.
 *
 * Details:
 * - Traverses the list and applies `f` to the content of each node.
 */
void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
