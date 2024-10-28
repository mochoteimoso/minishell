/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:09:31 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/24 11:34:28 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static	size_t	count_words(char const *s, char c)
{
	size_t	words;

	words = 0;
	if (!s)
		return (0);
	while (*s)
	{
		if (*s && *s != c)
		{
			words++;
			while (*s && *s != c)
				s++;
		}
		while (*s && *s == c)
			s++;
	}
	return (words);
}

static void	free_array(char **array, size_t i)
{
	while (i > 0)
	{
		i--;
		free(array[i]);
	}
	free(array);
}

static char	**separator(char const *s, char c, size_t i, char **result)
{
	char const	*start;

	while (*s)
	{
		if (*s && *s != c)
		{
			start = s;
			while (*s && *s != c)
				s++;
			result[i] = (char *)malloc(sizeof(char) * (s - start + 1));
			if (!result[i])
			{
				free_array(result, i);
				return (NULL);
			}
			ft_strlcpy(result[i], start, s - start + 1);
			i++;
		}
		while (*s && *s == c)
			s++;
	}
	result[i] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	char	**result;

	i = 0;
	result = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!s || !result)
		return (NULL);
	return (separator(s, c, i, result));
}
