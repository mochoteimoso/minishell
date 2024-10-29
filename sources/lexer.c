/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:09:08 by henbuska          #+#    #+#             */
/*   Updated: 2024/10/29 18:55:47 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	lexer_cmd_line(char *str)
{
	if (check_single_quote)
		return (1);
	if (check_double_quote)
		return (1);
}

int	check_single_quote(char *str)
{
	int		quote_count;
	char	single_quote;
	
	quote_count = 0;
	single_quote = '\'';
	
	while (*str)
	{
		if (*str == single_quote)
			quote_count++;
		str++;
	}
	printf("%d\n", quote_count);
	if (quote_count % 2 == 0)
		return (0);
	else
		return (1);
}

int	check_double_quote(char *str)
{
	int		quote_count;
	char	double_quote;
	
	quote_count = 0;
	double_quote = '"';
	while (*str)
	{
		if (*str== double_quote)
			quote_count++;
		str++;
	}
	printf("%d\n", quote_count);
	if (quote_count % 2 == 0)
		return (0);
	else
		return (1);
}