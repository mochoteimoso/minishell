/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_name.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:06:10 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/20 15:01:56 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_cmd_name(t_shell *mini, t_cmd *cmd, int i);
int quoted_cmd(t_expand *name, char *segment);

// Retrieves command name from string and copies it to struct

int quoted_cmd(t_expand *name, char *segment)
{
	what_quote(segment, name);
	while (segment[name->i])
	{
		// printf("result value: {%s}\n", result->value);
		if (segment[name->i] == ' ' && !name->sgl && !name->dbl)
			break ;
		else if (!name->sgl && !name->dbl && (segment[name->i] == '\''
			|| segment[name->i] == '"'))
		{
			what_quote(segment, name);
		}
		else if ((name->sgl && segment[name->i] == '\'')
			|| (name->dbl && segment[name->i] == '"'))
		{
			what_quote(segment, name);
		}
		else if (add_char(segment, name))
			return (free(name->value), -1);
	}
	name->len = ft_strlen(name->value);
	return (name->i);
}

int	handle_cmd_name(t_shell *mini, t_cmd *cmd, int i)
{
	t_expand	name;

	(void)mini;
	i = skip_whitespace(cmd->segment, i);
	the_arg(&name, i);
	while (cmd->segment[name.i])
	{
		// printf("segment[%d]: {%c}\n", name.i, cmd->segment[name.i]);
		if (cmd->segment[name.i] == ' ' || cmd->segment[name.i] == '<'
			|| cmd->segment[name.i] == '>' || cmd->segment[name.i] == '|')
			break ;
		if (cmd->segment[name.i] == '\'' || cmd->segment[name.i] == '"')
		{
			name.i = quoted_cmd(&name, cmd->segment);
			if (ft_strlen(name.value) == 0)
			{
				cmd->command = ft_strdup("''");
				return (name.i);
			}
			break ;
		}
		else if (add_char(cmd->segment, &name))
			return (free(cmd->segment), 1);
	}
	cmd->command = ft_strdup(name.value);
	return (name.i);
}
