/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:05:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/18 10:36:24 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		split_input_by_pipes(char *input, t_shell *mini);
char	*trim_whitespace(char *segment);


// splits input string by pipes and copies each segment into the segment member
// of the relevant struct in the array
static int	segment_handler(t_shell *mini, char *input, int start, int end, int *index)
{
	char	*trimmed;

	trimmed = ft_strndup(input + start, end - start);
	if (!trimmed)
	{
		printf("Failed to allocate memory for trimmed string");
		return (1);
	}
	trimmed = trim_whitespace(trimmed);
	if (!mini->cmds[*index])
	{
		printf("No segment in struct\n");
		free(trimmed);
		return (1);
	}
	mini->cmds[*index]->segment = trimmed;
	(*index)++;
	return (0);
}

static int segment_end(t_shell *mini, char *input, int start, int end, int *index)
{
	char	*trimmed;

	trimmed = trim_whitespace(ft_strndup(input + start, end - start));
	if (!trimmed)
	{
		printf("Failed to allocate memory for trimmed string");
		return (1);
	}
	mini->cmds[*index]->segment = trimmed;
	return (0);
}

int	split_input_by_pipes(char *input, t_shell *mini)
{
	int		i;
	int		start;
	int		index;

	i = 0;
	start = 0;
	index = 0;
	while (input[i])
	{
		if (input[i] == '|' && !check_quotes(input, i))
		{
			if (segment_handler(mini, input, start, i, &index))
				return (1);
			start = i + 1;
		}
		i++;
	}
	if (segment_end(mini, input,  start, i, &index))
		return (1);
	return (0);
}

//trims leading and trailing whitespaces from the segment string

char	*trim_whitespace(char *segment)
{
	int		end;
	int		start;
	int		len;

	start = 0;
	end = ft_strlen(segment) - 1;
	while (segment[start] && ft_isspace(segment[start]))
		start++;
	while (end >= start && ft_isspace(segment[end]))
		end--;
	len = end - start + 1;
	ft_memmove(segment, segment + start, len + 1);
	segment[len] = '\0';
	return (segment);
}
