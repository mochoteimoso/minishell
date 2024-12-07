/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_filename.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:54:32 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/07 14:12:30 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_filename(t_cmd *cmd, int i, char **filename);
int	filename_in_quotes(char *str, int i, t_expand *arg);

int	parse_filename(t_cmd *cmd, int i, char **filename)
{
	t_expand	arg;

	arg.sgl = 0;
	arg.dbl = 0;
	arg.len = 0;
	arg.i = i;
	if (filename_in_quotes(cmd->segment, arg.i, &arg) == -1)
		return (-1);
	*filename = ft_strdup(arg.value);
	if (!*filename)
	{
		free(arg.value);
		ft_putendl_fd("Memory allocation for filename failed", 2);
		return (-1);
	}
	i = arg.i;
	free(arg.value);
	return (i);
}

int	filename_in_quotes(char *str, int i, t_expand *arg)
{
	i = skip_whitespace(str, i);
	if (the_arg(arg, i))
		return (-1);
	what_quote(str, arg);
	while (str[arg->i])
	{
		if (str[arg->i] == ' ' && !arg->sgl && !arg->dbl)
			break ;
		else if (!arg->sgl && !arg->dbl && (str[arg->i] == '\'' || str[arg->i] == '"'))
			what_quote(str, arg);
		else if ((arg->sgl && str[arg->i] == '\'') || (arg->dbl && str[arg->i] == '"'))
			what_quote(str, arg);
		else if (add_char(str, arg))
			return (free(arg->value), -1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}
