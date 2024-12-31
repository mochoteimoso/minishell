/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:05:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/31 17:23:49 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			split_input_by_pipes(char *input, t_shell *mini);
char		*trim_whitespace(char *seg);
static int	segment_handler(t_cmd *cmd, char *input, int s, int e);

/**
 * split_input_by_pipes - Splits the input string by
 * 						  unquoted pipe ('|') characters.
 *
 * @input: The input string to parse and split.
 * @mini: Pointer to the shell structure containing command information.
 *
 * Iterates through the input string, identifying pipe-separated segments while
 * respecting quotes. Calls `segment_handler` for each segment to trim whitespace
 * and assign it to the corresponding command structure. Updates the `cmds` array
 * in `mini` with these segments. Returns 0 on success, or 1 on failure.
 */
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

/**
 * trim_whitespace - Removes leading and trailing whitespace from a string.
 *
 * @seg: The input string to trim.
 *
 * Finds the first and last non-whitespace characters in the string, shifts the
 * relevant portion to the start of the buffer, and null-terminates the result.
 * Modifies the input string in-place. Returns the trimmed string.
 */
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

/**
 * segment_handler - Processes and trims a segment of the input string.
 *
 * @cmd: Pointer to the command structure where the segment will be stored.
 * @input: The input string containing the segment.
 * @s: Start index of the segment in the input string.
 * @e: End index of the segment in the input string.
 *
 * Extracts a substring from the input string between indices `s` and `e`, trims
 * any leading or trailing whitespace, and assigns it to the `seg` field of the
 * command structure. Handles memory allocation errors and returns 1 on failure,
 * 0 on success.
 */
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
