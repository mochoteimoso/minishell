/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 08:55:39 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:29:14 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/get_next_line.h"

/**
 * create_list - Reads data from the file descriptor and builds a linked list.
 *
 * @list: Pointer to the head of the buffer list.
 * @fd: File descriptor to read from.
 *
 * Details:
 * - Continuously reads `BUFFER_SIZE` bytes from the file descriptor.
 * - Appends the data to the linked list until a newline is found
 *   or EOF is reached.
 * - Frees any allocated memory if an error occurs.
 */
void	create_list(t_gnl_list **list, int fd)
{
	int		c;
	char	*buffer;

	c = 1;
	while (!find_nl(*list) && c != 0)
	{
		buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer)
		{
			list_emptier(list, NULL);
			return ;
		}
		c = read(fd, buffer, BUFFER_SIZE);
		if (c == -1 || c == 0)
		{
			free(buffer);
			buffer = NULL;
			if (!(*list) && c == 0)
				list_emptier(list, NULL);
			return ;
		}
		buffer[c] = '\0';
		appender(list, buffer, c);
		free(buffer);
	}
}

/**
 * appender - Appends a new node containing a buffer to the linked list.
 *
 * @list: Pointer to the head of the buffer list.
 * @buffer: The buffer containing data read from the file.
 * @c: Number of characters in the buffer to copy.
 *
 * Returns:
 * The head of the linked list (`*list`), which includes the newly appended node.
 *
 * Details:
 * - Allocates memory for the new node and its buffer.
 * - Copies `c` characters from `buffer` to the node's buffer.
 * - Links the new node to the end of the list.
 */
t_gnl_list	*appender(t_gnl_list **list, char *buffer, int c)
{
	int			i;
	t_gnl_list	*new_n;
	t_gnl_list	*last_n;

	new_n = malloc(sizeof(t_gnl_list));
	if (!new_n)
		return (*list);
	new_n->l_buffer = malloc(sizeof(char) * (c + 1));
	if (!new_n->l_buffer)
	{
		list_emptier(list, &new_n);
		return (*list);
	}
	new_n->next = NULL;
	i = -1;
	while (buffer[++i] && i < c)
		new_n->l_buffer[i] = buffer[i];
	new_n->l_buffer[i] = '\0';
	if (!(*list))
		return (*list = new_n);
	last_n = lstlast(*list);
	last_n->next = new_n;
	return (*list);
}

/**
 * lstlast - Finds the last node in the linked list.
 *
 * @list: Pointer to the head of the buffer list.
 *
 * Returns:
 * Pointer to the last node in the list.
 */
t_gnl_list	*lstlast(t_gnl_list *list)
{
	t_gnl_list	*node;

	node = list;
	while (node && node->next)
		node = node->next;
	return (node);
}

/**
 * reset_list - Creates a new list containing the leftover buffer
 * 				after a newline.
 *
 * @list: Pointer to the head of the buffer list.
 *
 * Details:
 * - Allocates a new node to store data remaining after the newline.
 * - Copies leftover characters from the last node into the new node.
 * - Frees the old list and updates `*list` to point to the new node.
 */
void	reset_list(t_gnl_list **list)
{
	t_gnl_list	*clean_n;
	t_gnl_list	*last_n;

	clean_n = malloc(sizeof(t_gnl_list));
	if (!clean_n)
	{
		list_emptier(list, NULL);
		return ;
	}
	clean_n->next = NULL;
	last_n = lstlast(*list);
	clean_n->l_buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!last_n || !clean_n->l_buffer)
	{
		if (!clean_n->l_buffer)
			list_emptier(list, &clean_n);
		return ;
	}
	read_old(clean_n, last_n);
	list_emptier(list, NULL);
	*list = clean_n;
}

/**
 * list_emptier - Frees all nodes in the linked list
 * 				  and optionally a specific node.
 *
 * @list: Pointer to the head of the buffer list.
 * @node: Pointer to a specific node to free (optional).
 *
 * Details:
 * - Frees all allocated buffers and nodes in the list.
 * - If `node` is provided, it is also freed separately.
 */
void	list_emptier(t_gnl_list **list, t_gnl_list **node)
{
	t_gnl_list	*temp;
	t_gnl_list	*next;

	if (node)
	{
		free(*node);
		*node = NULL;
	}
	if (list)
	{
		temp = *list;
		while (temp)
		{
			free(temp->l_buffer);
			next = temp->next;
			free(temp);
			temp = next;
		}
		*list = NULL;
	}
}
