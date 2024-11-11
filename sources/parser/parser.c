/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/11 19:56:56 by henbuska         ###   ########.fr       */
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
		int j = 0;
		while (mini->cmds[i]->args[j] != NULL)
		{
			printf("Struct %d: arg %d: %s\n", i, j, mini->cmds[i]->args[j]);
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
		//printf("Struct %d: redirect_in: %u\n", i, mini->cmds[i]->redir_tail->type);
		//printf("Struct %d: redirect_out %s\n", i, mini->cmds[i]->redir_tail->file);
		//printf("Struct %d: redirect_out %s\n", i, mini->cmds[i]->redir_tail->file);
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

/* int parse_cmd_string(t_cmd *cmd)
{
	int		i;
	bool	cmd_found ;
	
	i = 0;
	cmd_found = false;
	while (cmd->segment[i])
	{
		// Step 1: Handle initial redirections
		if (is_redirection(cmd, i)) 
		{
			i = handle_redirections(cmd, i);
			if (i == -1)
				return (1);
		}
		// Step 2: Find the command if not found yet
		else if (!cmd_found) 
		{
			i = handle_cmd_name(cmd, i);
			if (i == -1)
				return (1);
			cmd_found = true;
		}
		// Step 3: After command is found, handle arguments
		else if (cmd_found && !is_redirection(cmd, i))
		{
			i = handle_cmd_args(cmd, i);
			if (i == -1)
				return (1);
		}
		// Step 4: Handle any redirections after arguments
		else if (is_redirection(cmd, i))
		{
			i = handle_redirections(cmd, i);
			if (i == -1)
				return (1);
		}
		else
			i++;  // Move to the next character if none of the above conditions are met
	}
	return (0);
} */

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
	i = handle_redirections(cmd, i);
	if (i == -1)
		return (1);
	return (0);
}

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


