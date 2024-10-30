/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:40:55 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/30 16:05:17 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../includes/minishell.h"
#include <stdio.h>

int	check_quotes(char *str)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*str)
	{
		if (*str == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*str == '"' && !in_single_quote)
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
	int	piped;

	i = 0;
	piped = 0;
	while (str[i])
	{
		if (str[i] == '|')
		{
			piped = 1;
			while (str[i] == ' ')
				i++;
			if (piped == 1 && str[i] == '|')
			{
				printf("syntax error near unexpected token %c\n", str[i]);
				return (1);
			}
		i++;
		}
	}
}

int	lexer_cmd_line(char *str)
{
	if (!*str)
		return (1);
	if (check_quotes(str))
		return (1);
	if (check_pipes(str))
		return (1);
	else
		return (0);
}

int	main(int argc, char **argv)
{
	argc = argc;
	printf("%d\n",lexer_cmd_line(argv[1]));
	return (0);
}

