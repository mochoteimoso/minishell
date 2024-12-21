/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:05:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/21 19:48:03 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		split_input_by_pipes(char *input, t_shell *mini);
char	*trim_whitespace(char *seg);

static int	segment_handler(t_shell *mini, char *input, int start, int end, int *index)
{
	char	*trimmed;

	trimmed = ft_strndup(input + start, end - start);
	if (!trimmed)
	{
		ft_putendl_fd("Failed to allocate memory for trimmed string", 2);
		return (1);
	}
	trimmed = trim_whitespace(trimmed);
	mini->cmds[*index]->seg = trimmed;
	if (!mini->cmds[*index])
	{
		ft_putendl_fd("No seg in struct", 2);
		free(trimmed);
		return (1);
	}
	(*index)++;
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
	if (segment_handler(mini, input,  start, i, &index))
		return (1);
	return (0);
}

//trims leading and trailing whitespaces from the seg string

char	*trim_whitespace(char *seg)
{
	int		end;
	int		start;
	int		len;

	if (!seg)
		return (NULL);
	start = 0;
	end = ft_strlen(seg) - 1;
	while (seg[start] && ft_isspace(seg[start]))
		start++;
	while (end >= start && ft_isspace(seg[end]))
		end--;
	len = end - start + 1;
	ft_memmove(seg, seg + start, len + 1);
	seg[len] = '\0';
	return (seg);
}
