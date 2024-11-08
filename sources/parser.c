/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/08 15:33:23 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strndup(const char *src, size_t n);
int		parse_input(t_shell *sh);
int		parse_cmd_string(t_shell *sh, int index);
int		handle_redirections(char *cmd_string, int i, t_shell *sh, int index);
int		handle_cmd_name(char *cmd_string, int i, t_shell *sh, int index);

// Allocates memory and  duplicates a string

char	*ft_strndup(const char *src, size_t n)
{
	size_t	src_len;
	size_t	size;
	size_t	i;
	char	*dest;

	src_len = ft_strlen(src);
	if (src_len < n)
		size = src_len;
	else
		size = n;
	dest = (char *) malloc(size * sizeof(char) + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[size] = '\0';
	return (dest);
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
	i = handle_cmd_name(cmd_string, i, sh, index);
	if (i == -1)
		return (1);
	while (cmd_string[i])
	{
		if (is_redirection(cmd_string, i))
		{
			i = handle_redirections(cmd_string, i, sh, index);
			if (i == -1)
				return (1);
		}	
		else if (!cmd_found)
		{
			i = handle_cmd_name(cmd_string, i, sh, index);
			if (i == -1)
				return (1);
			cmd_found = true;
		}
		//else
		//{
		//  i = handle_cmd_args(cmd_string, i, sh, index);
		//	if (i == -1)
		//		return (1);
		//}
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
		else if (!ft_isspace(cmd_string[i]))
			break ;
		(i)++;
	}
	printf("index after handle_redirections: %d\n", i);
	return (i);
}

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

/*int	handle_cmd_args(char *cmd_string, int i, t_shell *sh, int index)
{
	int		arg_length = 0;
	char	*arg_start;
	
	while (cmd_string[i] && ft_isspace(cmd_string[i]))
		i++;
	arg_start = &cmd_string[i];
	while (cmd_string[i] && !ft_isspace(cmd_string[i]) && !is_redirection(cmd_string, i))
	{
		arg_length++;
		i++;
	}
} */

/*int	count_args(char *cmd_string, int i)
{
	int	count;
	int i;
	
} */