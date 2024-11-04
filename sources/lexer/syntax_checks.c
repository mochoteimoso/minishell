/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:45:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/04 15:02:52 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	validate_input_syntax(char *line);
int	is_in_quotes(char *line, int i);
int	check_consecutive_pipes(char *line);
int	check_pipes(char *line);
int	check_redirects(char *line);
int	validate_redirect(char *line, int *i, char *type);

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
	if (*line == ';' || *line == '\\')
	{
		printf("invalid syntax");
		return (1);
	}
	if (check_redirects(line))
		return (1);
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
		if (line[i] == '\'')
			in_single_quote = !in_single_quote;
		else if (line[i] == '"')
			in_double_quote = !in_double_quote;
		line++;
	}
	if (in_double_quote || in_single_quote)
		return (1);
	else
		return (0);
}

/*int	is_in_quotes(char *line, int i)
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
} */

int	check_pipes(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '|' && !is_in_quotes(line, i))
	{
		printf("syntax error near unexpected token %c\n", line[i]);
		return (1);
	}
	if (check_consecutive_pipes(line))
	{
		printf("syntax error near unexpected token %c\n", line[i]);
		return (1);
	}
	while (line[i])
		i++;
	while (i >= 0 && line[i - 1] == ' ')
		i--;
	if (line[i - 1] == '|' && !is_in_quotes(line, i - 1))
	{
		printf("syntax error near unexpected token %c\n", line[i]);
		return (1);
	}
	return (0);
}

int	check_consecutive_pipes(char *line)
{
	int	i;
	int	pipe_found;

	i = 0;
	pipe_found = 0;
	while (line[i])
	{
		if (line[i] == '|' && !is_in_quotes(line, i))
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

int	check_redirects(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '>' && !is_in_quotes(line, i))
		{
			if (line[i + 1] == '>')
			{
				i++;
				if (validate_redirect(line, &i, ">>") != 0)
					return (1);
			}
			else
			{
				if (validate_redirect(line, &i, ">") != 0)
					return (1);
			}
		}
		else if (line[i] == '<' && !is_in_quotes(line, i))
		{
			if (line[i + 1] == '<')
			{
				i++;
				if (validate_redirect(line, &i, "<<") != 0)
					return (1);
			}
			else
			{
				if (validate_redirect(line, &i, "<") != 0)
					return (1);
			}
		}
		i++;
	}
	return (0);
}

int	validate_redirect(char *line, int *i, char *type)
{
	(*i)++;
	while (line[*i] == ' ')
		(*i)++;
	if (!line[*i] || line[*i] == '|'|| line[*i] == '<' || line[*i] == '>')
	{
		printf("syntax error near unexpected token %s\n", type);
		return (1);
	}
	return (0);
}
