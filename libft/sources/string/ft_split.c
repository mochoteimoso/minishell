/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:09:31 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:50:39 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/**
 * count_words - Counts the number of words in a string separated by a delimiter.
 *
 * @s: String to analyze.
 * @c: Delimiter character.
 *
 * Returns:
 * The number of words in the string.
 *
 * Details:
 * - Iterates through the string, identifying words as sequences
 * 	 of characters not equal to `c`.
 */
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

/**
 * free_array - Frees a NULL-terminated array of strings.
 *
 * @array: Array of strings to free.
 * @i: Number of strings to free (used during error handling).
 *
 * Details:
 * - Frees each string and the array itself.
 */
static void	free_array(char **array, size_t i)
{
	while (i > 0)
	{
		i--;
		free(array[i]);
	}
	free(array);
}

/**
 * separator - Splits the string into words based on a delimiter.
 *
 * @s: String to split.
 * @c: Delimiter character.
 * @i: Index for result array.
 * @result: Array to store the resulting words.
 *
 * Returns:
 * A pointer to the array of words, or NULL on allocation failure.
 *
 * Details:
 * - Allocates memory for each word and copies it into the array.
 * - Frees allocated memory on failure.
 */
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

/**
 * ft_split - Splits a string into an array of words separated by a delimiter.
 *
 * @s: String to split.
 * @c: Delimiter character.
 *
 * Returns:
 * A NULL-terminated array of words, or NULL on allocation failure.
 *
 * Details:
 * - Allocates memory for the array and splits the string using `separator`.
 * - Uses `count_words` to determine the number of words.
 */
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
