/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:09:08 by henbuska          #+#    #+#             */
/*   Updated: 2024/10/31 15:03:33 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	check_line_syntax(char *line);
int	check_quotes(char *line);
int	check_pipes(char *line);
int	check_consecutive_oper(char *line);

int	check_line_syntax(char *line)
{
	if (!*line)
		return (1);
	if (check_quotes(line))
		return (1);
	if (check_pipes(line))
		return (1);
	if (line == ';' || line == '\\')
	{
		printf("Invalid syntax");
		return (1);
	}
	else
		return (0);
}

int	check_quotes(char *line)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*line)
	{
		if (*line == '\'' && in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*line == '"' && in_single_quote)
			in_double_quote = !in_double_quote;
		line++;
	}
	if (in_double_quote || in_single_quote)
		return (1);
	else
		return (0);	
}

int	check_pipes(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '|')
	{
		printf("syntax error near unexpected token %c\n", line[i]);
		return (1);
	}
	if (check_consecutive_oper(line))
	{
		printf("syntax error near unexpected token %c\n", line[i]);
		return (1);
	}
	while (line[i])
		i++;
	while (i >= 0 && line[i] == ' ')
		i--;
	if (line[i] == '|')
	{
		printf("syntax error near unexpected token %c\n", line[i]);
		return (1);
	}
	return (0);
}

int	check_consecutive_oper(char *line)
{
	int	i;
	int	pipe_found;

	i = 0;
	pipe_found = 0;
	while (line[i])
	{
		if (line[i] == '|')
		{
			if (pipe_found)
			{
				printf("syntax error near unexpected token %c\n", line[i]);
				return (1);
			}
			pipe_found = 1;
		}
		else if (line[i] == ' ')
			pipe_found = 0;
		i++;
	}
	return (0);
}
