/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_filename.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:54:32 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/03 19:19:10 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_filename_length(t_cmd *cmd, int i, bool in_quotes)
{
	int	len;

	len = 0;
	while (cmd->segment[i] && ((!in_quotes && !ft_isspace(cmd->segment[i])
		&& !is_redirection(cmd, i)) ||(in_quotes && cmd->segment[i] != '"' 
		&& cmd->segment[i] != '\'')))
	{
		len++;
		i++;
	}
	return (len);
}

int	parse_filename(t_cmd *cmd, int i, bool *in_quotes, char **filename)
{
	int		filename_len;
	char	*filename_start;

	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	if (cmd->segment[i] == '"' || cmd->segment[i] == '\'')
	{
		*in_quotes = true;
		i++;
	}
	filename_start = &cmd->segment[i];
	filename_len = get_filename_length(cmd, i, *in_quotes);
	i += filename_len;
	if (*in_quotes && (cmd->segment[i] == '"' || cmd->segment[i] == '\''))
		i++;
	*filename = ft_strndup(filename_start, filename_len);
	if (!*filename)
	{
		ft_putendl_fd("Memory allocation for filename failed", 2);
		return (-1);
	}
	return (i);
}
