/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:57:16 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 12:55:08 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			generate_hd_file(t_cmd *cmd);
static int	index_to_char(t_cmd *cmd, t_hd *hd);
static int	create_name(t_cmd *cmd, t_hd *hd);

/**
 * generate_hd_file - Generates a unique file name
 * 					  for the heredoc temporary file.
 *
 * @cmd: Pointer to the command structure.
 *
 * Combines command and heredoc indices into a unique file name for the heredoc.
 * Cleans up resources and unlinks any pre-existing file with the same name.
 * Returns 0 on success or 1 on failure.
 */
int	generate_hd_file(t_cmd *cmd)
{
	t_hd	hd;

	ft_memset(&hd, 0, sizeof(t_hd));
	if (index_to_char(cmd, &hd))
		return (1);
	if (create_name(cmd, &hd))
	{
		if (hd.cmd_str)
			free(hd.cmd_str);
		if (hd.heredoc_str)
			free(hd.heredoc_str);
		if (hd.base)
			free(hd.base);
		if (hd.mid)
			free(hd.mid);
		if (hd.full)
			free(hd.full);
		return (1);
	}
	free(hd.full);
	unlink(cmd->redir_tail->heredoc_name);
	return (0);
}

/**
 * index_to_char - Converts command and heredoc indices into strings.
 *
 * @cmd: Pointer to the command structure.
 * @hd: Pointer to the heredoc data structure containing string buffers.
 *
 * Allocates memory and converts indices to strings for further use in file name
 * generation. Returns 0 on success or 1 on failure.
 */
static int	index_to_char(t_cmd *cmd, t_hd *hd)
{
	hd->cmd_str = ft_itoa(cmd->cmd_index);
	if (!(hd->cmd_str))
		return (1);
	hd->heredoc_str = ft_itoa(cmd->redir_head->heredoc_index);
	if (!(hd->heredoc_str))
	{
		free(hd->cmd_str);
		return (1);
	}
	return (0);
}

/**
 * create_name - Constructs the heredoc file name using indices and constants.
 *
 * @cmd: Pointer to the command structure.
 * @hd: Pointer to the heredoc data structure containing string buffers.
 *
 * Combines strings and constants to create the unique heredoc file name.
 * Frees intermediate strings during the process.
 * Returns 0 on success or 1 on failure.
 */
static int	create_name(t_cmd *cmd, t_hd *hd)
{
	hd->base = ft_strjoin(TMP_S, hd->cmd_str);
	if (!hd->base)
		return (1);
	free(hd->cmd_str);
	hd->mid = ft_strjoin(hd->base, "_");
	if (!hd->mid)
		return (1);
	free(hd->base);
	hd->full = ft_strjoin(hd->mid, hd->heredoc_str);
	if (!hd->full)
		return (1);
	free(hd->mid);
	free(hd->heredoc_str);
	cmd->redir_tail->heredoc_name = ft_strjoin(hd->full, TMP_EXT);
	if (!cmd->redir_tail->heredoc_name)
		return (1);
	return (0);
}
