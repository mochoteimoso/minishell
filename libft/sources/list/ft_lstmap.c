/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:23:21 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:52:04 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lstmap - Creates a new list by applying a function to each node's content.
 *
 * @lst: Pointer to the head of the source list.
 * @f: Function to apply to each node's content.
 * @del: Function to delete the content of a node in case of allocation failure.
 *
 * Returns:
 * A pointer to the head of the new list or NULL if allocation fails.
 *
 * Details:
 * - Allocates and creates a new node for each transformed content.
 * - Frees the new list using `del` if memory allocation fails.
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_l;
	t_list	*new_n;
	void	*set;

	if (!lst || !f || !del)
		return (NULL);
	new_l = NULL;
	while (lst)
	{
		set = f(lst->content);
		new_n = ft_lstnew(set);
		if (!new_n)
		{
			del(set);
			ft_lstclear(&new_l, (*del));
			return (new_l);
		}
		ft_lstadd_back(&new_l, new_n);
		lst = lst->next;
	}
	return (new_l);
}
