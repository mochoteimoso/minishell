/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:05:32 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/02 11:44:21 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			handle_redirections(t_shell *mini, t_cmd *cmd, int i);
static int	single_redirect(t_cmd *cmd, int i);
static int	double_redirect(t_shell *mini, t_cmd *cmd, int i);

/**
 * handle_redirections - Parses and processes redirections
 * 						 in the command segment.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Pointer to the command structure.
 * @i: Index in the command segment to start processing.
 *
 * Iterates through the command segment,
 * handling both single (`<`, `>`) and double (`<<`, `>>`) redirections.
 * Depending on the type of redirection,
 * it delegates processing to the appropriate
 * functions. Handles updating the redirection linked list
 * and ensures no syntax errors occur.
 * Returns the updated index on success, or -1 on failure.
 */
int	handle_redirections(t_shell *mini, t_cmd *cmd, int i)
{
	while (i < (int)ft_strlen(cmd->seg) && cmd->seg[i])
	{
		if (is_redirection(cmd, i))
		{
			if (redirll_head_tail(cmd))
				return (-1);
			if ((i + 1 < (int)ft_strlen(cmd->seg))
				&& ((cmd->seg[i] == '<' && cmd->seg[i + 1] == '<')
					|| (cmd->seg[i] == '>' && cmd->seg[i + 1] == '>')))
			{
				i = double_redirect(mini, cmd, i);
				if (i == -1)
					return (-1);
			}
			else if (cmd->seg[i] == '<' || cmd->seg[i] == '>')
			{
				i = single_redirect(cmd, i);
				if (i == -1)
					return (-1);
			}
		}
		else
			break ;
	}
	return (i);
}

/**
 * single_redirect - Handles single redirections `<` or `>`
 * 					 in the command segment.
 *
 * @cmd: Pointer to the command structure.
 * @i: Index in the command segment where the single redirection starts.
 *
 * Depending on the redirection type, calls either `handle_redirect_in`
 * for input redirection `<`
 * or `handle_redirect_out` for output redirection `>`.
 * Returns the updated index on success, or -1 on failure.
 */
static int	single_redirect(t_cmd *cmd, int i)
{
	if (cmd->seg[i] == '<')
	{
		i = handle_redirect_in(cmd, i);
		if (i == -1)
			return (-1);
	}
	else if (cmd->seg[i] == '>')
	{
		i = handle_redirect_out(cmd, i);
		if (i == -1)
			return (-1);
	}
	return (i);
}

/**
 * double_redirect - Handles double redirections `<<` or `>>`
 * 					 in the command segment.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Pointer to the command structure.
 * @i: Index in the command segment where the double redirection starts.
 *
 * Depending on the redirection type, calls either `handle_heredoc`
 * for heredoc redirection `<<`
 * or `handle_append` for append redirection `>>`.
 * Returns the updated index on success, or -1 on failure.
 */
static int	double_redirect(t_shell *mini, t_cmd *cmd, int i)
{
	if (cmd->seg[i] == '<' && cmd->seg[i + 1] == '<')
	{
		i = handle_heredoc(mini, cmd, i);
		if (i == -1)
			return (-1);
	}
	else if (cmd->seg[i] == '>' && cmd->seg[i + 1] == '>')
	{
		i = handle_append(cmd, i);
		if (i == -1)
			return (-1);
	}
	return (i);
}
