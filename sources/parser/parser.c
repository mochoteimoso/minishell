/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/09 14:50:34 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strndup(const char *src, size_t n);
int		parse_input(t_shell *sh);
int		parse_cmd_string(t_shell *sh, int index);
int		handle_redirections(char *cmd_string, int i, t_shell *sh, int index);
int		handle_cmd_name(char *cmd_string, int i, t_shell *sh, int index);

// Validates input string and parses the content into an array of structs

int	parse_and_validate_input(char *input, t_shell *sh)
{
	if (validate_input_syntax(input))
		return (1);
	if (prepare_command_structs(sh, input))
		return (1);
	if (split_input_by_pipes(input, sh))
		return (1);
	if (parse_input(sh))
		return (1);
	int i = 0;
	while (sh->cmds[i])
	{
		printf("\n");
		printf("Struct %d: segment: %s\n", i, sh->cmds[i]->segment);
		printf("Struct %d: command: %s\n", i, sh->cmds[i]->command);
		printf("Struct %d: arg 1: %s\n", i, sh->cmds[i]->args[0]);
		printf("Struct %d: arg 2: %s\n", i, sh->cmds[i]->args[1]);
		printf("Struct %d: redirect_in: %s\n", i, sh->cmds[i]->redirect_in);
		printf("Struct %d: redirect_out %s\n", i, sh->cmds[i]->redirect_out);
		printf("Struct %d: append %s\n", i, sh->cmds[i]->append);
		printf("Struct %d: heredoc: %d\n", i, sh->cmds[i]->heredoc);
		printf("Struct %d: heredoc_delim: %s\n", i, sh->cmds[i]->heredoc_delim);
		printf("\n");
		i++;
	}
	return (0);
}

// Parses information added to array of structs

int	parse_input(t_shell *sh)
{
	int	index;

	index = 0;
	while (sh->cmds[index])
	{
		if (parse_cmd_string(sh, index))
			return (1);
		index++;
	}
	return (0);
}

// Parses the segment string of each struct 

// create a linked list if the segment string contains redirections
// each redirect will be its own node and will contain information about redirection type,
// filename, delimiter and pointer to next node
// move these items from the array of structs to the linked list

int	parse_cmd_string(t_shell *sh, int index)
{
	int		i;
	char	*cmd_string;
	bool	cmd_found;

	i = 0;
	cmd_found = false;
	cmd_string = sh->cmds[index]->segment;
	i = handle_redirections(cmd_string, i, sh, index);
	if (i == -1)
		return ((1));
	i = handle_cmd_name(cmd_string, i, sh, index);
	if (i == -1)
		return (1);
	cmd_found = true;
	while (cmd_string[i] && cmd_found && !is_redirection(cmd_string, i))
	{
		i = handle_cmd_args(cmd_string, i, sh, index);
		if (i == -1)
			return (1);
	}
	while (cmd_string[i])
	{
		if (is_redirection(cmd_string, i))
		{
			i = handle_redirections(cmd_string, i, sh, index);
			if (i == -1)
				return (1);
		}	
		else
			i++;
	}
	return (0);
}


// Handles different redirections in the segment strings

int	handle_redirections(char *cmd_string, int i, t_shell *sh, int index)
{
	while (cmd_string[i])
	{
		if (cmd_string[i] == '<' && cmd_string[i + 1] == '<')
		{
			if (handle_heredoc(cmd_string, &i, sh, index))
				return (-1);
		}
		else if (cmd_string[i] == '>' && cmd_string[i + 1] == '>')
		{
			if (handle_append(cmd_string, &i, sh, index))
				return (-1);
		}
		else if (cmd_string[i] == '<')
		{
			if (handle_redirect_in(cmd_string, &i, sh, index))
				return (-1);
		}
		else if (cmd_string[i] == '>')
		{
			if (handle_redirect_out(cmd_string, &i, sh, index))
				return (-1);
		}
		else
			break ;
		i++;
	}
	printf("index after handle_redirections: %d\n", i);
	return (i);
}

// Retrieves command name from string and copies it to struct

int	handle_cmd_name(char *cmd_string, int i, t_shell *sh, int index)
{
	char	*cmd_start;
	int		cmd_length;
	
	cmd_length = 0;
	while (cmd_string[i] && ft_isspace(cmd_string[i]))
		i++;
	if (cmd_string[i] == '\'' || cmd_string[i] == '"')
		i++;
	cmd_start = &cmd_string[i];
 	while (cmd_string[i] && !ft_isspace(cmd_string[i]) && !is_redirection(cmd_string, i))
	{
		cmd_length++;
		i++;
	}
	sh->cmds[index]->command = ft_strndup(cmd_start, cmd_length);
	if (!sh->cmds[index]->command)
	{
		printf("Failed to allocate memory for command name\n");
		return (-1);
	}
	printf("index after handle_command_name: %d\n", i);
	return (i);
}


