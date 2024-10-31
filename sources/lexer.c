/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:09:08 by henbuska          #+#    #+#             */
/*   Updated: 2024/10/31 12:52:08 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	check_line_syntax(char *str);
int	check_quotes(char *str);
int	check_pipes(char *str);
int	check_consecutive_oper(char *str);

int	check_line_syntax(char *str)
{
	if (!*str)
		return (1);
	if (check_quotes(str))
		return (1);
	if (check_pipes(str))
		return (1);
	if (str == ';' || str == '\\')
	{
		printf("Invalid syntax");
		return (1);
	}
	else
		return (0);
}

int	check_quotes(char *str)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*str)
	{
		if (*str == '\'' && in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*str == '"' && in_single_quote)
			in_double_quote = !in_double_quote;
		str++;
	}
	if (in_double_quote || in_single_quote)
		return (1);
	else
		return (0);	
}

int	check_pipes(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '|')
	{
		printf("syntax error near unexpected token %c\n", str[i]);
		return (1);
	}
	if (check_consecutive_oper(str))
	{
		printf("syntax error near unexpected token %c\n", str[i]);
		return (1);
	}
	while (str[i])
		i++;
	while (i >= 0 && str[i] == ' ')
		i--;
	if (str[i] == '|')
	{
		printf("syntax error near unexpected token %c\n", str[i]);
		return (1);
	}
	return (0);
}

int	check_consecutive_oper(char *str)
{
	int	i;
	int	pipe_found;

	i = 0;
	pipe_found = 0;
	while (str[i])
	{
		if (str[i] == '|')
		{
			if (pipe_found)
			{
				printf("syntax error near unexpected token %c\n", str[i]);
				return (1);
			}
			pipe_found = 1;
		}
		else if (str[i] == ' ')
			pipe_found = 0;
		i++;
	}
	return (0);
}
