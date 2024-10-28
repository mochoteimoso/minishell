/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 08:54:42 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/24 11:36:05 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/get_next_line.h"

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
