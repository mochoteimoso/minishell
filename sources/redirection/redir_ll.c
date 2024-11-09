/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_ll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:42:04 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/09 14:46:12 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	redir_lstadd_back(t_redir **lst, t_redir *new)
{
	t_redir	*temp;

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

static t_redir	*redir_add_node(void)
{
	t_redir	*node;

	node = (t_redir *)malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->delimiter = NULL;
	node->file = NULL;
	node->type = NULL;
	node->next = NULL;
	return (node);
}

t_redir	*list_redir(void)
{
	t_redir	*ll;
	t_redir	*new;

	ll = NULL;
	new = redir_add_node();
	redir_lstadd_back(&ll, new);
	return (ll);
}
