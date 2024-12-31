/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_ll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:42:04 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 17:30:05 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int				redirll_head_tail(t_cmd *cmd);
static void		redir_lstadd_back(t_redir **lst, t_redir *new);
static t_redir	*redir_add_node(void);
static t_redir	*list_redir(void);
static void		redir_update_tail(t_cmd *cmd);

/**
 * redirll_head_tail - Initializes or updates the head
 * 					   and tail of the redirection linked list.
 *
 * @cmd: Pointer to the command structure.
 *
 * If the redirection list is empty, initializes the `redir_head`
 * and `redir_tail` by creating a new redirection node.
 * If the list already exists, adds a new node to the end
 * and updates `redir_tail`. Prints an error message and returns -1 if
 * memory allocation fails, otherwise returns 0.
 */
int	redirll_head_tail(t_cmd *cmd)
{
	if (!cmd->redir_head)
	{
		cmd->redir_head = list_redir();
		if (!cmd->redir_head)
		{
			ft_putendl_fd("Failed to initialize redirection list", 2);
			return (-1);
		}
		cmd->redir_tail = cmd->redir_head;
	}
	else
	{
		redir_update_tail(cmd);
		if (!cmd->redir_tail)
		{
			ft_putendl_fd("Failed to allocate memory for new redir node", 2);
			return (-1);
		}
	}
	return (0);
}

/**
 * redir_lstadd_back - Adds a new node to the end of a redirection linked list.
 *
 * @lst: Pointer to the head of the redirection list.
 * @new: Pointer to the new redirection node to add.
 *
 * If the list is empty, initializes it with the new node.
 * Otherwise, iterates to the end of the list and appends the new node.
 * Does nothing if `lst` or `new` is NULL.
 */
void	redir_lstadd_back(t_redir **lst, t_redir *new)
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

/**
 * redir_add_node - Creates a new redirection node.
 *
 * Allocates memory for a new redirection node
 * and initializes its fields to default values:
 * - `delimiter` and `file` set to NULL.
 * - `type` set to 0, `expand` set to true.
 * - `heredoc_name` set to NULL, `heredoc_index` set to 0.
 * - `next` set to NULL.
 *
 * Returns a pointer to the newly created node,
 * or NULL if memory allocation fails.
 */
static t_redir	*redir_add_node(void)
{
	t_redir	*node;

	node = (t_redir *)malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->delimiter = NULL;
	node->file = NULL;
	node->type = 0;
	node->expand = true;
	node->heredoc_name = NULL;
	node->heredoc_index = 0;
	node->next = NULL;
	return (node);
}

/**
 * list_redir - Creates and initializes a new redirection linked list.
 *
 * Creates a new redirection node using `redir_add_node` and adds it to the list
 * as the head. Returns a pointer to the head of the list, or NULL if memory
 * allocation fails.
 */
static t_redir	*list_redir(void)
{
	t_redir	*ll;
	t_redir	*new;

	ll = NULL;
	new = redir_add_node();
	redir_lstadd_back(&ll, new);
	return (ll);
}

/**
 * redir_update_tail - Adds a new redirection node to the end of the list
 * 					   and updates the tail.
 *
 * @cmd: Pointer to the command structure containing the redirection list.
 *
 * Creates a new redirection node and appends it to the existing list.
 * Updates the `redir_tail` pointer to point to the new node.
 * Does nothing if memory allocation fails.
 */
static void	redir_update_tail(t_cmd *cmd)
{
	t_redir	*new;
	t_redir	*temp;

	temp = NULL;
	new = redir_add_node();
	redir_lstadd_back(&cmd->redir_head, new);
	temp = cmd->redir_head;
	while (temp->next)
		temp = temp->next;
	cmd->redir_tail = temp;
}
