/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:09:08 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/01 16:21:44 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	validate_input_syntax(char *line);
int	is_in_quotes(char *line, int i);

int	validate_input_syntax(char *line)
{
	if (!*line)
		return (1);
	if (is_in_quotes(line, 0))
	{
		printf("syntax error: unmatched quotes");
		return (1);
	}
	if (check_pipes(line))
		return (1);
	if (line == ';' || line == '\\')
	{
		printf("invalid syntax");
		return (1);
	}
	else
		return (0);
}

int	is_in_quotes(char *line, int i)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*line)
	{
		if (line[i] == '\'' && in_double_quote)
			in_single_quote = !in_single_quote;
		else if (line[i] == '"' && in_single_quote)
			in_double_quote = !in_double_quote;
		line++;
	}
	if (in_double_quote || in_single_quote)
		return (1);
	else
		return (0);	
}


int	check_redirects(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '>')
		{
			if (line[i + 1] == '>')
			{
				if (validate_redirect(line, &i, '>>') != 0)
					return (1);
			}
			else
			{
				if (validate_redirect(line, &i, '>') != 0)
					return (1);
			}
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				if (validate_redirect(line, &i, '<<') != 0)
					return (1);
			}
			else
			{
				if (validate_redirect(line, &i, '<') != 0)
					return (1);
			}
		}
		i++;
	}
}

int	validate_redirect(char *line, int *i, char type)
{
	(*i)++;
	if (type == '>>' || type == '<<')
		(*i)++;
	while (line[*i] == ' ')
		(*i)++;
	if (!line[*i])
	
}