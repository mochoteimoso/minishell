/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:05:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 11:19:24 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			split_input_by_pipes(char *input, t_shell *mini);
char		*trim_whitespace(char *seg);
static int	segment_handler(t_cmd *cmd, char *input, int s, int e);

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
			if (segment_handler(mini->cmds[index], input, start, i))
				return (1);
			start = i + 1;
			index++;
		}
		i++;
	}
	if (segment_handler(mini->cmds[index], input, start, i))
		return (1);
	index++;
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

static int	segment_handler(t_cmd *cmd, char *input, int s, int e)
{
	char	*trimmed;

	trimmed = ft_strndup(input + s, e - s);
	if (!trimmed)
	{
		ft_putendl_fd("Failed to allocate memory for trimmed string", 2);
		return (1);
	}
	trimmed = trim_whitespace(trimmed);
	cmd->seg = trimmed;
	if (!cmd->seg)
	{
		ft_putendl_fd("No seg in struct", 2);
		free(trimmed);
		return (1);
	}
	return (0);
}
