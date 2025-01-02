/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:01:48 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:54:59 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_redirection(t_cmd *cmd, int i);
int		handle_redirect_in(t_cmd *cmd, int i);
int		handle_redirect_out(t_cmd *cmd, int i);
int		handle_heredoc(t_shell *mini, t_cmd *cmd, int i);
int		handle_append(t_cmd *cmd, int i);

/**
 * is_redirection - Determines if the current segment character is a redirection.
 *
 * @cmd: Pointer to the command structure containing the segment.
 * @i: Index in the segment to check.
 *
 * Returns true if the character at the specified index is
 * a redirection symbol (`<` or `>`)
 * and is not within quotes; otherwise, returns false.
 */
bool	is_redirection(t_cmd *cmd, int i)
{
	if ((cmd->seg[i] == '>' || cmd->seg[i] == '<')
		&& !check_quotes(cmd->seg, i))
		return (true);
	else
		return (false);
}

/**
 * handle_redirect_in - Handles the `<` redirection in the command segment.
 *
 * @cmd: Pointer to the command structure.
 * @i: Index in the command segment where the redirection starts.
 *
 * Parses the filename following the `<` symbol,
 * assigns it to the `file` field of the
 * current redirection node, and sets the redirection type to `REDIRECT_IN`.
 * Returns the updated index on success, or -1 on failure.
 */
int	handle_redirect_in(t_cmd *cmd, int i)
{
	char	*filename;

	filename = NULL;
	i++;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = REDIRECT_IN;
	return (i);
}

/**
 * handle_redirect_out - Handles the `>` redirection in the command segment.
 *
 * @cmd: Pointer to the command structure.
 * @i: Index in the command segment where the redirection starts.
 *
 * Parses the filename following the `>` symbol,
 * assigns it to the `file` field of the
 * current redirection node, and sets the redirection type to `REDIRECT_OUT`.
 * Returns the updated index on success, or -1 on failure.
 */
int	handle_redirect_out(t_cmd *cmd, int i)
{
	char	*filename;

	filename = NULL;
	i++;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = REDIRECT_OUT;
	return (i);
}

/**
 * handle_heredoc - Handles the `<<` heredoc redirection in the command segment.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Pointer to the command structure.
 * @i: Index in the command segment where the heredoc starts.
 *
 * Parses the delimiter following the `<<` symbol,
 * assigns it to the `delimiter` field of the
 * current redirection node, and sets the redirection type to `HEREDOC`.
 * Handles the heredoc
 * content by generating a temporary file and writing the heredoc lines to it.
 * Returns the updated index on success, or -1 on failure.
 */
int	handle_heredoc(t_shell *mini, t_cmd *cmd, int i)
{
	char	*delim;

	delim = NULL;
	i += 2;
	if (cmd->seg[i] == '\'' || cmd->seg[i] == '"')
		cmd->redir_tail->expand = false;
	i = parse_filename(cmd, i, &delim);
	if (i == -1 || !delim)
		return (-1);
	cmd->redir_tail->delimiter = delim;
	cmd->redir_tail->type = HEREDOC;
	if (generate_hd_file(cmd))
		return (-1);
	if (open_and_write_to_heredoc(mini, cmd))
		return (-1);
	return (i);
}

/**
 * handle_append - Handles the `>>` append redirection in the command segment.
 *
 * @cmd: Pointer to the command structure.
 * @i: Index in the command segment where the append redirection starts.
 *
 * Parses the filename following the `>>` symbol,
 * assigns it to the `file` field of the
 * current redirection node, and sets the redirection type to `APPEND`.
 * Returns the updated index on success, or -1 on failure.
 */
int	handle_append(t_cmd *cmd, int i)
{
	char	*filename;

	filename = NULL;
	i += 2;
	i = parse_filename(cmd, i, &filename);
	if (i == -1 || !filename)
		return (-1);
	cmd->redir_tail->file = filename;
	cmd->redir_tail->type = APPEND;
	return (i);
}
