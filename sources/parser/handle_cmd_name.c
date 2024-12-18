/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_name.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:06:10 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/17 19:11:57 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_cmd_name(t_shell *mini, t_cmd *cmd, int i);
int	skip_to_next_segment(t_shell *mini, t_cmd *cmd, int i);
int	process_quoted_segment(char *segment, int i, t_expand *result);

/*int	handle_cmd_name(t_shell *mini, t_cmd *cmd, int i)
{
	t_expand	cmd_name;

	i = skip_whitespace(cmd->segment, i);
	i = process_quoted_segment(mini, cmd->segment, i, &cmd_name);
	if (i == -1)
		return (-1);
	cmd->command = ft_strdup(cmd_name.value);
	free(cmd_name.value);
	if (!cmd->command)
	{
		ft_putendl_fd("Failed to allocate memory for command name", 2);
		return (-1);
	}
	return cmd_name.i;
} */

int	handle_cmd_name(t_shell *mini, t_cmd *cmd, int i)
{
	t_expand cmd_name;

	i = skip_whitespace(cmd->segment, i);
	i = process_quoted_segment(cmd->segment, i, &cmd_name);
	if (i == -1)
		return (-1);
	if (cmd_name.value && ft_strlen(cmd_name.value) == 0)
	{
		free(cmd_name.value);
		i = skip_to_next_segment(mini, cmd, i);
		if (!cmd->segment[i])
			return (i);
		if (process_quoted_segment(cmd->segment, i, &cmd_name) == -1)
			return (-1);
	}
	cmd->command = ft_strdup(cmd_name.value);
	free(cmd_name.value);
	if (!cmd->command)
	{
		ft_putendl_fd("Failed to allocate memory for command name", 2);
		return (-1);
	}
	printf("cmd->command %s\n", cmd->command);
	return (cmd_name.i);
}

/*int	handle_cmd_name(t_shell *mini, t_cmd *cmd, int i)
{
	t_expand	cmd_name;
	char 		**split_value;
	int			j;

	i = skip_whitespace(cmd->segment, i);
	i = process_quoted_segment(mini, cmd->segment, i, &cmd_name);
	if (i == -1)
		return (-1);
	if (cmd_name.value && ft_strlen(cmd_name.value) == 0)
	{
		free(cmd_name.value);
		i = skip_to_next_segment(mini, cmd, i);
		if (!cmd->segment[i])
			return (i);
		if (process_quoted_segment(mini, cmd->segment, i, &cmd_name) == -1)
			return (-1);
	}
	split_value = ft_split(cmd_name.value, ' ');
	if (!split_value || !split_value[0])
	{
		free(cmd_name.value);
		return (-1);
	}
	cmd->command = ft_strdup(split_value[0]);
	if (!cmd->command)
	{
		ft_putendl_fd("Failed to allocate memory for command name", 2);
		ft_free_array(split_value);
		free(cmd_name.value);
		return (-1);
	}
	j = 1;
	while (split_value[j])
	{
		if (add_to_args(cmd, split_value[j]) == -1)
		{
			ft_free_array(split_value);
			free(cmd_name.value);
			return (-1);
		}
		j++;
	}
	ft_free_array(split_value);
	free(cmd_name.value);
	return (cmd_name.i);
}

int	add_to_args(t_cmd *cmd, char *arg)
{
	char **new_args;
	int i = 0;
	
	while (cmd->args && cmd->args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
	{
		ft_putendl_fd("Failed to allocate memory for arguments", 2);
		return (-1);
	}
	int j = 0;
	while (j < i)
	{
		new_args[j] = cmd->args[j];
		j++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
	{
		ft_putendl_fd("Failed to allocate memory for argument", 2);
		free(new_args);
		return (-1);
	}
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (0);
} */

int	process_quoted_segment(char *segment, int i, t_expand *result)
{
	if (the_arg(result, i))
		return (-1);
	what_quote(segment, result);
	while (segment[result->i])
	{
		if ((segment[result->i] == ' ' || segment[result->i] == '<'
			|| segment[result->i] == '>') && (!result->sgl && !result->dbl))
			break;
		else if (!result->sgl && !result->dbl && (segment[result->i] == '\''
			|| segment[result->i] == '"'))
			what_quote(segment, result);
		else if ((result->sgl && segment[result->i] == '\'')
			|| (result->dbl && segment[result->i] == '"'))
			what_quote(segment, result);
		else if (add_char(segment, result))
			return (free(result->value), -1);
	}
	result->len = ft_strlen(result->value);
	return (result->i);
}

int	skip_to_next_segment(t_shell *mini, t_cmd *cmd, int i)
{
	while (cmd->segment[i])
	{
		i = skip_whitespace(cmd->segment, i);
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(mini, cmd, i);
			if (i == -1)
				return (-1);
			continue;
		}
		if (!ft_isspace(cmd->segment[i]))
			break;
	}
	return (i);
}

// Retrieves command name from string and copies it to struct

/*int	handle_cmd_name(t_cmd *cmd, int i)
{
	char	*cmd_start;
	int		cmd_length;

	cmd_length = 0;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
		i++;
	cmd_start = &cmd->segment[i];
 	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) &&
	!is_redirection(cmd, i))
	{
		cmd_length++;
		i++;
	}
	cmd->command = ft_strndup(cmd_start, cmd_length);
	if (!cmd->command)
	{
		ft_putendl_fd("Failed to allocate memory for command name", 2);
		return (-1);
	}
	return (i);
} */