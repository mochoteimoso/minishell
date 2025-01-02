/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 08:54:42 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:20:00 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/get_next_line.h"

/**
 * find_nl - Checks if a newline character exists in the list's buffer.
 *
 * @list: Pointer to the head of the buffer list.
 *
 * Returns:
 * 1 if a newline character is found, 0 otherwise.
 */
int	find_nl(t_gnl_list *list)
{
	int			i;
	t_gnl_list	*line;

	if (!list)
		return (0);
	line = lstlast(list);
	i = 0;
	while (line->l_buffer[i])
	{
		if (line->l_buffer[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

/**
 * getting_line - Extracts the line from the buffer list up to the newline.
 *
 * @list: Pointer to the buffer list.
 * @next_l: Pointer to the resulting line.
 *
 * Allocates memory for `next_l` and copies characters from the buffer list
 * up to (and including) the newline character.
 */
void	getting_line(t_gnl_list *list, char **next_l)
{
	int	i;
	int	j;
	int	len;

	len = line_till_nl(list);
	*next_l = malloc(sizeof(char) * (len + 1));
	if (!(*next_l))
		return ;
	j = 0;
	while (list)
	{
		i = 0;
		while (list->l_buffer[i])
		{
			if (list->l_buffer[i] == '\n')
			{
				(*next_l)[j++] = list->l_buffer[i];
				break ;
			}
			(*next_l)[j++] = list->l_buffer[i++];
		}
		list = list->next;
	}
	(*next_l)[j] = '\0';
}

/**
 * line_till_nl - Calculates the length of the line up to the newline.
 *
 * @list: Pointer to the buffer list.
 *
 * Returns:
 * The length of the line including the newline, or the total length if
 * no newline is found.
 */
int	line_till_nl(t_gnl_list *list)
{
	int	i;
	int	len;

	if (!list)
		return (0);
	len = 0;
	while (list)
	{
		i = 0;
		while (list->l_buffer[i])
		{
			if (list->l_buffer[i] == '\n')
			{
				len++;
				return (len);
			}
			len++;
			i++;
		}
		list = list->next;
	}
	return (len);
}

/**
 * read_old - Removes processed characters from the buffer after a newline.
 *
 * @clean_n: Pointer to the cleaned node where the leftover buffer
 * 			 will be stored.
 * @last_n: Pointer to the last node of the buffer list containing the old data.
 *
 * Copies characters from after the newline in `last_n` to `clean_n`.
 */
void	read_old(t_gnl_list *clean_n, t_gnl_list *last_n)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (last_n->l_buffer[i] && last_n->l_buffer[i] != '\n')
		i++;
	if (last_n->l_buffer && last_n->l_buffer[i] == '\n')
		i++;
	while (last_n->l_buffer[i])
		clean_n->l_buffer[j++] = last_n->l_buffer[i++];
	clean_n->l_buffer[j] = '\0';
}

/**
 * get_next_line - Reads a line from the file descriptor.
 *
 * @fd: File descriptor to read from.
 *
 * Returns:
 * A pointer to the line read, or NULL if an error occurs or the file ends.
 *
 * Details:
 * - Manages a static linked list (`list`) to store data across calls.
 * - Calls helper functions to read, extract, and reset the buffer.
 */
char	*get_next_line(int fd)
{
	static t_gnl_list	*list = NULL;
	char				*next_l;

	if (fd < 0 || BUFFER_SIZE < 0 || read(fd, &next_l, 0) < 0)
	{
		list_emptier(&list, NULL);
		return (NULL);
	}
	create_list(&list, fd);
	if (!list)
		return (NULL);
	getting_line(list, &next_l);
	reset_list(&list);
	if (next_l && next_l[0] == '\0')
	{
		free(next_l);
		next_l = NULL;
	}
	if (!next_l)
		list_emptier(&list, NULL);
	return (next_l);
}
