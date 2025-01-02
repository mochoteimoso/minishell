/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_filename.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:54:32 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:55:51 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_filename(t_cmd *cmd, int i, char **filename);
int	filename_in_quotes(t_cmd *cmd, char *str, int i, t_expand *arg);

/**
 * parse_filename - Parses and extracts a filename from the command segment.
 *
 * @cmd: Pointer to the command structure containing the segment to parse.
 * @i: Current index in the segment where filename parsing starts.
 * @filename: Pointer to store the extracted filename.
 *
 * Uses `filename_in_quotes` to handle quoted or unquoted filenames,
 * and copies the result into the `filename` parameter.
 * Handles memory allocation errors and updates
 * the index `i` to reflect the end of the parsed filename.
 * Returns the updated index on success, or -1 on failure.
 */
int	parse_filename(t_cmd *cmd, int i, char **filename)
{
	t_expand	arg;

	arg.i = i;
	if (filename_in_quotes(cmd, cmd->seg, arg.i, &arg) == -1)
		return (-1);
	*filename = ft_strdup(arg.value);
	if (!*filename)
	{
		free(arg.value);
		ft_putendl_fd("Memory allocation for filename failed", 2);
		return (-1);
	}
	i = arg.i;
	free(arg.value);
	return (i);
}

/**
 * filename_in_quotes - Extracts a filename while
 * 						respecting quotes in the segment.
 *
 * @cmd: Pointer to the command structure, used to track redirection properties.
 * @str: The segment string being parsed.
 * @i: Current index in the segment.
 * @arg: Pointer to the expansion structure for
 * 		 managing quotes and intermediate data.
 *
 * Handles single and double quotes during filename parsing,
 * updating the `expand` flag in the command's redirection
 * structure when quoted. Stops parsing on encountering
 * whitespace or redirection operators outside of quotes. Updates `arg` to store
 * the extracted filename and its length. Returns the updated index on success,
 * or -1 on failure.
 */
int	filename_in_quotes(t_cmd *cmd, char *str, int i, t_expand *arg)
{
	i = skip_whitespace(str, i);
	if (the_arg(arg, i))
		return (-1);
	what_quote(str, arg);
	if (arg->sgl == 1 || arg->dbl == 1)
		cmd->redir_tail->expand = false;
	while (str[arg->i])
	{
		if ((str[arg->i] == ' ' || str[arg->i] == '<' || str[arg->i] == '>')
			&& (!arg->sgl && !arg->dbl))
			break ;
		else if (!arg->sgl && !arg->dbl && (str[arg->i] == '\''
				|| str[arg->i] == '"'))
			what_quote(str, arg);
		else if ((arg->sgl && str[arg->i] == '\'')
			|| (arg->dbl && str[arg->i] == '"'))
			what_quote(str, arg);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}
