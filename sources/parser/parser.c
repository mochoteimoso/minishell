/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/11 13:23:10 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strndup(const char *src, size_t n);
int		parse_input(t_shell *mini);
int		parse_cmd_string(t_cmd *cmd);
int		handle_redirections(t_cmd *cmd, int i);
int		handle_cmd_name(t_cmd *cmd, int i);

// Validates input string and parses the content into an array of structs

int	parse_and_validate_input(char *input, t_shell *mini)
{
	if (validate_input_syntax(input))
		return (1);
	if (prepare_command_structs(mini, input))
		return (1);
	if (split_input_by_pipes(input, mini))
		return (1);
	if (parse_input(mini))
		return (1);
	int i = 0;
	while (mini->cmds[i])
	{
		printf("\n");
		printf("Struct %d: segment: %s\n", i, mini->cmds[i]->segment);
		printf("Struct %d: command: %s\n", i, mini->cmds[i]->command);
		printf("Struct %d: arg 1: %s\n", i, mini->cmds[i]->args[0]);
		printf("Struct %d: arg 2: %s\n", i, mini->cmds[i]->args[1]);
		printf("Struct %d: redirect_in: %u\n", i, mini->cmds[i]->redir->type);
		printf("Struct %d: redirect_out %s\n", i, mini->cmds[i]->redir->file);
		//printf("Struct %d: append %s\n", i, mini->cmds[i]->append);
		//printf("Struct %d: heredoc: %d\n", i, mini->cmds[i]->heredoc);
		//printf("Struct %d: heredoc_delim: %s\n", i, mini->cmds[i]->heredoc_delim);
		printf("\n");
		i++;
	}
	return (0);
}

// Parses information added to array of structs

int	parse_input(t_shell *mini)
{
	int	index;

	index = 0;
	while (mini->cmds[index])
	{
		if (parse_cmd_string(mini->cmds[index]))
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

int	parse_cmd_string(t_cmd *cmd)
{
	int		i;
	bool	cmd_found;

	i = 0;
	cmd_found = false;
	
	i = handle_redirections(cmd, i);
	if (i == -1)
		return ((1));
	i = handle_cmd_name(cmd, i);
	if (i == -1)
		return (1);
	cmd_found = true;
	while (cmd->segment[i] && cmd_found && !is_redirection(cmd, i))
	{
		i = handle_cmd_args(cmd, i);
		if (i == -1)
			return (1);
	}
	while (cmd->segment[i])
	{
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(cmd, i);
			if (i == -1)
				return (1);
		}	
		else
			i++;
	}
	return (0);
}

// Handles different redirections in the segment strings

int	handle_redirections(t_cmd *cmd, int i)
{
	t_redir *new;
	t_redir *temp;
	
	if (is_redirection(cmd, i))
	{
		if (!cmd->redir)
		{
			cmd->redir = list_redir(cmd);
			if (!cmd->redir) {
				printf("Failed to initialize redirection list\n");
				return (-1);
			}
		}
		else
		{
			new = redir_add_node();
			if (!new) 
			{
				printf("Failed to allocate memory for new redirection node\n");
				return (-1);
			}
		}
	redir_lstadd_back(&cmd->redir, new);
	//temp = cmd->redir->next;
	while (cmd->segment[i])
	{
		if (cmd->segment[i] == '<' && cmd->segment[i + 1] == '<')
		{	
			if (handle_heredoc(cmd, i, new) == -1)
				return (-1);
		}
		else if (cmd->segment[i] == '>' && cmd->segment[i + 1] == '>')
		{
			if (handle_append(cmd, i, new) == -1)
				return (-1);
		}
		else if (cmd->segment[i] == '<')
		{
			if (handle_redirect_in(cmd, i, new) == -1)
				return (-1);
		}
		else if (cmd->segment[i] == '>')
		{
			if (handle_redirect_out(cmd, i, new) == -1)
				return (-1);
		}
		else
			break ;
			i++;
		}
	}
	printf("index after handle_redirections: %d\n", i);
	return (i);
}

// Retrieves command name from string and copies it to struct

int	handle_cmd_name(t_cmd *cmd, int i)
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
		printf("Failed to allocate memory for command name\n");
		return (-1);
	}
	printf("index after handle_command_name: %d\n", i);
	return (i);
}


