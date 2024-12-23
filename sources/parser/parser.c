/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/23 18:28:06 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_and_validate_input(char **input, t_shell *mini);
int	parse_input(t_shell *mini);
int	parse_cmd_string(t_shell *mini, t_cmd *cmd);
int	cmd_string_while(t_shell *mini, t_cmd *cmd, int i, int *cmd_found);

// Validates input string and parses the content into an array of structs

int	parse_and_validate_input(char **input, t_shell *mini)
{
	if (validate_input_syntax(input, mini))
		return (1);
	if (prepare_command_structs(mini, *input))
		return (1);
	if (split_input_by_pipes(*input, mini))
		return (1);
	if (parse_input(mini))
		return (1);
	return (0);
}

int	parse_input(t_shell *mini)
{
	int	index;

	index = 0;
	while (mini->cmds[index])
	{
		if (parse_cmd_string(mini, mini->cmds[index]))
		{
			mini->exit_stat = 1;
			clean_cmds(mini->cmds);
			return (1);
		}
		index++;
	}
	return (0);
}

int	parse_cmd_string(t_shell *mini, t_cmd *cmd)
{
	int	i;
	int	cmd_found;

	i = 0;
	cmd_found = 0;
	if (handle_expand(mini, &cmd))
		return (1);
	i = cmd_string_while(mini, cmd, i, &cmd_found);
	if (i == -1)
		return (1);
	if (cmd_found && (!cmd->args || !cmd->args[0]))
		i = no_args(cmd, i);
	return (0);
}

int	cmd_string_while(t_shell *mini, t_cmd *cmd, int i, int *cmd_found)
{
	while (cmd->seg[i])
	{
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(mini, cmd, i);
			if (i == -1)
				return (-1);
		}
		else if (*cmd_found == 0)
		{
			i = handle_cmd_name(cmd, i);
			if (i == -1)
				return (-1);
			*cmd_found = 1;
		}
		else
		{
			i = handle_cmd_args(mini, cmd, i);
			if (i == -1)
				return (-1);
		}
	}
	return (i);
}
