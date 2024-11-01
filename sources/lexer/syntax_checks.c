/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:45:48 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/01 16:22:13 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	check_pipes(char *line);
int	check_consecutive_pipes(char *line);

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


int	check_consecutive_pipes(char *line)
{
	int	i;
	int	oper_found;

	i = 0;
	oper_found = 0;
	while (line[i])
	{
		if (line[i] == operator && !is_in_quotes(line, i))
		{
			if (oper_found)
			{
				printf("syntax error near unexpected token %c\n", line[i]);
				return (1);
			}
			oper_found = 1;
		}
		else if (line[i] == ' ')
			oper_found = 0;
		i++;
	}
	return (0);
}