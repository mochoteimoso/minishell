/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_name.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:35:20 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 15:13:10 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			handle_cmd_name(t_cmd *cmd, int i);
static int	quoted_cmd(t_expand *name, char *seg);
static int	this_is_redirection(t_cmd *cmd, t_expand *name);

int	handle_cmd_name(t_cmd *cmd, int i)
{
	t_expand	name;

	i = skip_whitespace(cmd->seg, i);
	the_arg(&name, i);
	while (cmd->seg[name.i])
	{
		if (this_is_redirection(cmd, &name))
			break ;
		if (cmd->seg[name.i] == '\'' || cmd->seg[name.i] == '"')
		{
			name.i = quoted_cmd(&name, cmd->seg);
			if (ft_strlen(name.value) == 0)
			{
				free(name.value);
				cmd->command = ft_strdup("''");
				return (name.i);
			}
			break ;
		}
		else if (add_char(cmd->seg, &name))
			return (free(cmd->seg), 1);
	}
	cmd->command = ft_strdup(name.value);
	free(name.value);
	return (name.i);
}

static int	this_is_redirection(t_cmd *cmd, t_expand *name)
{
	if (cmd->seg[name->i] == ' ' || cmd->seg[name->i] == '<'
		|| cmd->seg[name->i] == '>' || cmd->seg[name->i] == '|')
		return (1);
	else
		return (0);
}

static int	quoted_cmd(t_expand *name, char *seg)
{
	what_quote(seg, name);
	while (seg[name->i])
	{
		if (seg[name->i] == ' ' && !name->sgl && !name->dbl)
			break ;
		else if (!name->sgl && !name->dbl && (seg[name->i] == '\''
				|| seg[name->i] == '"'))
		{
			what_quote(seg, name);
		}
		else if ((name->sgl && seg[name->i] == '\'')
			|| (name->dbl && seg[name->i] == '"'))
		{
			what_quote(seg, name);
		}
		else if (add_char(seg, name))
			return (free(name->value), -1);
	}
	name->len = ft_strlen(name->value);
	return (name->i);
}
