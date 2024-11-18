/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */

/*   handle_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:26:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/12 11:32:12 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int	handle_cmd_args(t_cmd *cmd, int i);
int	count_args(t_cmd *cmd, int i);

// Counts how many command arguments the string contains
int	count_args(t_cmd *cmd, int i)
{
	int	args_count;

	args_count = 0;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	while (cmd->segment[i])
	{
		if (is_redirection(cmd, i))
		{
			i++;
			while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
				i++;
			while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) && is_redirection(cmd, i))
				i++;
		}
		else
		{
			args_count++;
			while (cmd->segment[i]&& !ft_isspace(cmd->segment[i]))
				i++;
		}
		while (cmd->segment[i]&& ft_isspace(cmd->segment[i]))
			i++;
	}
	return (args_count);
}

int	handle_cmd_args(t_cmd *cmd, int i)
{
	int		arg_len;
	int		arg_count;
	int		arg_index;
	char	*arg_start;

	arg_count = count_args(cmd, i);
	arg_index = 1;
	cmd->args = ft_calloc(arg_count + 2, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[0] = ft_strdup(cmd->command);
	i = skip_whitespace(cmd->segment, i);
	while (cmd->segment[i])
	{
		if (is_redirection(cmd, i))
		{
			i++;
			i = skip_whitespace(cmd->segment, i);
			if (cmd->segment[i] && !is_redirection(cmd, i))
			{
				arg_start = &cmd->segment[i];
				arg_len = 0;
				while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) && !is_redirection(cmd, i))
				{
					arg_len++;
					i++;
				}
			}
			continue ;
		}
		if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
			i = arg_in_quotes(cmd->segment, i, &arg_start, &arg_len);
		else
			i = arg_no_quotes(cmd, i, &arg_start, &arg_len);
		if (append_to_array(cmd, arg_start, arg_len, &arg_index) == -1)
			return (-1);
		i = skip_whitespace(cmd->segment, i);
	}
	cmd->args[arg_index] = NULL;
	return (i);
}
