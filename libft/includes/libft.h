/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:45:23 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/01 12:56:43 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "ft_printf.h"
# include "get_next_line.h"
# include "../../includes/minishell.h"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

char	*ft_itoa(int n);
char	**ft_split(char const *s, char c);
char	*ft_strchr(char const *s, int c);
char	*ft_strdup(char *src);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strnstr(char const *haystack, char const *needle, size_t num);
char	*ft_strrchr(char const *s, int c);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_array_len(char **str);
int		ft_atoi(char const *str);
int		ft_atoi_base(const char *str, int base);
int		ft_countchar(char *str, char c);
int		ft_tolower(int c);
int		ft_toupper(int c);
int		ft_isalnum(int ch);
int		ft_isalpha(int ch);
int		ft_isascii(int ch);
int		ft_isdigit(int ch);
int		ft_isprint(int ch);
int		ft_lstsize(t_list *lst);
int		ft_memcmp(char *ptr1, char *ptr2, size_t num);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(char const *s1, char const *s2, size_t num);
size_t	ft_strlcat(char *dest, char const *src, size_t size);
size_t	ft_strlcpy(char *dest, char const *src, size_t size);
size_t	ft_strlen(char const *str);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);
void	ft_bzero(void *ptr, size_t num);
void	*ft_calloc(size_t num_elem, size_t elem_size);
void	ft_free_array(char **array);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	*ft_memchr(void const *ptr, int ch, size_t num);
void	*ft_memcpy(void *dest, const void *src, size_t num);
void	*ft_memmove(void *dest, const void *src, size_t num);
void	*ft_memset(void *ptr, int value, size_t num);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
void	*ft_realloc(void *ptr, size_t old, size_t new);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));

#endif
