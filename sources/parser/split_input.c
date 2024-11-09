/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:05:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/09 14:53:40 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		split_input_by_pipes(char *input, t_shell *sh);
char	*trim_whitespace(char *segment);
int		ft_isspace(char c);

// splits input string by pipes and copies each segment into the segment member
// of the relevant struct in the array

int	split_input_by_pipes(char *input, t_shell *sh)
{
	int		i;
	int		start;
	int		index;
	char	*trimmed_segment;
	
	i = 0;
	start = 0;
	index = 0;
	
	while (input[i])
	{
		if (input[i] == '|' && !is_in_quotes(input, i))
		{
			trimmed_segment = trim_whitespace(ft_strndup(input + start, i - start));
			if (trimmed_segment == NULL)
			{
				printf("Failed to allocate memory for trimmed string");
				return (1);
			}
			sh->cmds[index]->segment = trimmed_segment;
			start = i + 1;
			index++;
		}
		i++;
	}
	trimmed_segment = trim_whitespace(ft_strndup(input + start, i - start));
	if (trimmed_segment == NULL)
	{
		printf("Failed to allocate memory for trimmed string");
		return (1);
	}
	sh->cmds[index]->segment = trimmed_segment;
	return (0);
}

//trims leading and trailing whitespaces from the segment string

char	*trim_whitespace(char *segment)
{
	int		i;
	int		end;
	int		start;
	int		len;
	char	*trimmed_segment;

	i = 0;
	start = 0;
	end = ft_strlen(segment) - 1;
	while (segment[start] && ft_isspace(segment[start]))
		start++;
	while (end >= start && ft_isspace(segment[end]))
		end--;
	len = end - start + 1;
	trimmed_segment = malloc(sizeof(char) * (len + 1));
	if (!trimmed_segment)
		return (NULL);
	while (i < len)
	{
		trimmed_segment[i] = segment[start + i];
		i++;
	}
	trimmed_segment[len] = '\0';
	return (trimmed_segment);
}

// checks if a character is a whitespace

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' )
	{
		return (1);
	}
	else
		return (0);
}
