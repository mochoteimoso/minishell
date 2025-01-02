/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:33:27 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 10:51:24 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * ft_lstdelone - Deletes a single node in a linked list.
 *
 * @lst: Pointer to the node to delete.
 * @del: Function to delete the content of the node.
 *
 * Details:
 * - Calls `del` to delete the content of the node.
 * - Frees the memory for the node itself.
 */
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
	lst = NULL;
}
