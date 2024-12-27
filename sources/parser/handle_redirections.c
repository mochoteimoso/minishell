/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:05:32 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/27 15:15:07 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			handle_redirections(t_shell *mini, t_cmd *cmd, int i);
static int	single_redirect(t_cmd *cmd, int i);
static int	double_redirect(t_shell *mini, t_cmd *cmd, int i);

/*Loops through seg string to find redirection symbols
creates a linked list if redirection symbol(s) are found
each redirect will be its own node and will contain
information about redirection type,
filename, delimiter and pointer to next node*/

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
