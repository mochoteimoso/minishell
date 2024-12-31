/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_ll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:42:04 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 11:22:14 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int				redirll_head_tail(t_cmd *cmd);
static void		redir_lstadd_back(t_redir **lst, t_redir *new);
static t_redir	*redir_add_node(void);
static t_redir	*list_redir(void);
static void		redir_update_tail(t_cmd *cmd);

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

static t_redir	*list_redir(void)
{
	t_redir	*ll;
	t_redir	*new;

	ll = NULL;
	new = redir_add_node();
	redir_lstadd_back(&ll, new);
	return (ll);
}

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
