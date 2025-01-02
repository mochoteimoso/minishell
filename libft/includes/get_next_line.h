/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 08:56:00 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 13:36:05 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

# include <unistd.h>
# include <stdlib.h>

typedef struct s_gnl_list
{
	char				*l_buffer;
	struct s_gnl_list	*next;
}	t_gnl_list;

	/*get_next_line.c*/

char		*get_next_line(int fd);
void		read_old(t_gnl_list *clean_n, t_gnl_list *last_n);
int			line_till_nl(t_gnl_list *list);
void		getting_line(t_gnl_list *list, char **next_l);
int			find_nl(t_gnl_list *list);

	/*get_next_line_utils.c*/

t_gnl_list	*appender(t_gnl_list **list, char *buffer, int c);
t_gnl_list	*lstlast(t_gnl_list *list);
void		create_list(t_gnl_list **list, int fd);
void		list_emptier(t_gnl_list **list, t_gnl_list **node);
void		reset_list(t_gnl_list **list);

#endif
