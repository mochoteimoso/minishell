/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/14 16:10:18 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strndup(const char *src, size_t n);
int		parse_input(t_shell *mini);
int		parse_cmd_string(t_shell *mini, t_cmd *cmd);
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
		printf("Struct %d: Segment: %s\n", i, mini->cmds[i]->segment);
		printf("Struct %d: Command: %s\n", i, mini->cmds[i]->command);
		printf("Struct %d: fd_in: %d\n", i, mini->cmds[i]->fd_in);
		printf("Struct %d: fd_out: %d\n", i, mini->cmds[i]->fd_out);
		int j = 0;
		while (mini->cmds[i]->args[j] != NULL)
		{
			printf("Struct %d: Arg %d: %s\n", i, j, mini->cmds[i]->args[j]);
			j++;
		}
		t_redir *redir = mini->cmds[i]->redir_head;
		int redir_index = 0;
		while (redir)
		{
			printf("Struct %d: Redirection %d - type: %d\n", i, redir_index, redir->type);
			printf("Struct %d: Redirection %d - file: %s\n",
				i, redir_index, redir->file ? redir->file : "(null)");
			printf("Struct %d: Redirection %d - delimiter: %s\n",
				i, redir_index, redir->delimiter ? redir->delimiter : "(null)");
			redir = redir->next;
			redir_index++;
		}
		printf("Struct %d: Command path: %s\n", i, mini->cmds[i]->cmd_path);
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
		if (parse_cmd_string(mini, mini->cmds[index]))
			return (1);
		index++;
	}
	return (0);
}

// Parses the segment string of each struct

int	parse_cmd_string(t_shell *mini, t_cmd *cmd)
{
	int		i;
	bool	cmd_found;

	i = 0;
	cmd_found = false;
	
	i = handle_redirections(cmd, i);
	printf("index after initial redirections: %d\n", i);
	if (i == -1)
		return ((1));
	i = handle_cmd_name(cmd, i);
	printf("index after command name: %d\n", i);
	if (i == -1)
		return (1);
	cmd_found = true;
	while (cmd->segment[i] && cmd_found && !is_redirection(cmd, i))
	{
		i = handle_cmd_args(cmd, i);
		printf("index after args: %d\n", i);
		if (i == -1)
			return (1);
	}
	i = handle_redirections(cmd, i);
	printf("index after final redirections: %d\n", i);
	if (i == -1)
		return (1);
	if (get_cmd_path(mini, cmd))
		return (1);
	if (resolve_fd(cmd) == -1)
		return (1);
	return (0);
}

//Loops through segment string to find redirection symbols
// creates a linked list if redirection symbol(s) are found
// each redirect will be its own node and will contain information about redirection type,
// filename, delimiter and pointer to next node

int handle_redirections(t_cmd *cmd, int i)
{
	while (cmd->segment[i])
	{
		// If a redirection symbol is found, create a new node and handle the redirection
		if (is_redirection(cmd, i))
		{
			// Create a new redirection node each time a redirection is encountered
			if (!cmd->redir_head)
			{
				cmd->redir_head = list_redir();  // Create the first redirection node
				if (!cmd->redir_head)
				{
					printf("Failed to initialize redirection list\n");
					return (-1);
				}
				cmd->redir_tail = cmd->redir_head;  // Set tail to head for the first node
			}
			else
			{
				// If the list already has redirections, update the tail and create a new node
				redir_update_tail(cmd);  // Update tail to point to the most recent redirection
				if (!cmd->redir_tail)
				{
					printf("Failed to allocate memory for new redirection node\n");
					return (-1);
				}
			}
			// Handle the specific redirection types
			if (cmd->segment[i] == '<' && cmd->segment[i + 1] == '<')
			{
				i = handle_heredoc(cmd, i);
				if (i == -1)
					return (-1);
			}
			else if (cmd->segment[i] == '>' && cmd->segment[i + 1] == '>')
			{
				i = handle_append(cmd, i);
				if (i == -1) 
					return (-1);
			}
			else if (cmd->segment[i] == '<')
			{
				i = handle_redirect_in(cmd, i);
				if (i == -1)
					return (-1);
			}
			else if (cmd->segment[i] == '>')
			{
				i = handle_redirect_out(cmd, i);
				if (i == -1)
					return (-1);
			}
			i++;
		}
		else
			break;
	}
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
	return (i);
}


